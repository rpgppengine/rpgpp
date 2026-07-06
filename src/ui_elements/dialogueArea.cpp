#include "ui_elements/dialogueArea.hpp"

#include "game.hpp"
#include "raymath.h"

DialogueArea::DialogueArea() : UIElement("DialogueArea") { init(); }

void DialogueArea::init() { props["text"] = ""; }

void DialogueArea::update() {
	if (line == nullptr) return;

	if (dialogue.lines.size() < lineIndex) return;
	if (line->sections.size() < sectionIndex) return;

	line = &dialogue.lines.at(lineIndex);
	section = &line->sections.at(sectionIndex);

	if (firstCharTyped == false) {
		firstCharTyped = true;
		auto &soundId = section->sound;
		Game::getSounds().playSound(soundId);
		return;
	}

	bool finished = false;
	if (charIndex == (text.size() - 1) || charIndex == text.size()) {
		finished = true;
	}

	finishedTyping = finished;

	// check for delay
	if (delay) {
		delayDuration -= (GetFrameTime() * static_cast<float>(60.0f / 20.0f));
		if (delayDuration <= 0.0f) {
			delay = false;
		}
	}

	// advancing char index
	frameCounter++;
	if (frameCounter > (60 / 20) && !delay) {
		frameCounter = 0;
		if (charIndex < text.size()) {
			charIndex++;

			// play sound
			if (charIndex < text.size()) {
				if (text.at(charIndex) != ' ') {
					auto &soundId = section->sound;
					Game::getSounds().playSound(soundId);
				}
			}
		}
	}
}

void DialogueArea::draw(Rectangle rect) {
	textPos = Vector2{0, 0};
	Vector2 charMeasure = Vector2{0, 0};

	for (int i = 0; i < charIndex; i++) {
		chooseSection(i, rect);

		if (maxLineHeight < charMeasure.y) {
			maxLineHeight = charMeasure.y;
		}

		if (font.path != section->font) {
			font.font = Game::getResources().getFont(section->font);
			font.path = section->font;
		}

		if (section->key == "delay" || section->delay > 0) {
			if (!delay) {
				delay = true;
				delayDuration = section->delay;
			}
		}

		if (!appliedTag) {
			if (section->padding > 0.0f) {
				charMeasure.x += padding;
				appliedTag = true;
			}
		}

		// draw the character
		const char *subText = TextSubtext(text.c_str(), i, 1);
		putChar(charMeasure, subText, rect);

		Vector2 newMeasure = MeasureTextEx(Game::getResources().getFont(section->font), TextSubtext(text.c_str(), i, 1),
										   section->textSize * 3, 1.0f);
		charMeasure = newMeasure;
	}
}

void DialogueArea::setDialogue(const DialogueBin &bin) {
	dialogueFinished = false;
	finishedTyping = false;

	this->dialogue = bin;
	this->lineIndex = 0;
	this->sectionIndex = 0;

	line = &this->dialogue.lines.at(lineIndex);
	section = &line->sections.at(sectionIndex);

	firstCharTyped = false;
	text = "";

	for (auto k : dialogue.lines.at(lineIndex).sections) {
		text = text.append(k.text);
	}

	this->frameCounter = 0;
	this->charIndex = 0;
}

void DialogueArea::putChar(Vector2 charMeasure, const char *c, Rectangle rect) {
	Vector2 a = Vector2Add(textPos, Vector2{charMeasure.x + 1, 0.0f});
	Vector2 finalCharPos = Vector2Add(Vector2{rect.x, rect.y}, a);

	bool hasNewline = false;
	if (!appliedTag && section->newline) {
		hasNewline = true;
	}
	if (TextIsEqual(c, "\n")) {
		hasNewline = true;
	}

	// Check for text overflow on x axis or for newline.
	if ((finalCharPos.x + charMeasure.x) > rect.width || hasNewline) {
		textPos.x = 0;
		textPos.y += charMeasure.y;

		a = textPos;
		finalCharPos = Vector2Add(Vector2{rect.x, rect.y}, a);
	} else {
		textPos.x += charMeasure.x;
	}

	DrawTextEx(Game::getResources().getFont(section->font), c, finalCharPos, section->textSize * 3, 1,
			   section->textColor);
}

void DialogueArea::chooseSection(int i, Rectangle rect) {
	int size = 0;
	int idx = 0;
	for (auto section : line->sections) {
		if (i < (size + TextLength(section.text.c_str()))) {
			if (sectionIndex != idx) {
				if (section.paddingMode == PADDING_PX) {
					padding = section.padding;
				} else {
					padding = rect.width * (section.padding / 100);
				}
				appliedTag = false;
				if (section.newline) {
					printf("newline tag.. \n");
				}
			}
			sectionIndex = idx;

			break;
		} else {
			size += TextLength(section.text.c_str());
		}
		idx++;
	}

	section = &line->sections.at(sectionIndex);
}

void DialogueArea::advanceToNextLine() {
	if (finishedTyping) {
		finishedTyping = false;

		if (lineIndex == (dialogue.lines.size() - 1)) {
			// hideDialogue();
			dialogueFinished = true;
		} else {
			lineIndex++;
			text = "";
			for (auto k : dialogue.lines.at(lineIndex).sections) {
				text = text.append(k.text);
			}

			charIndex = 0;
			firstCharTyped = false;
			sectionIndex = 0;

			textPos = Vector2{0, 0};

			appliedTag = false;
			padding = 0.0f;
			maxLineHeight = 0.0f;

			line = &dialogue.lines.at(lineIndex);
			section = &line->sections.at(sectionIndex);
		}
	} else {
		charIndex = (text.size() - 1);
	}
}
