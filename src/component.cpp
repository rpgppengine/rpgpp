#include "component.hpp"

#include <raymath.h>

#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "raylib.h"

void LabelComponent::loadFont(const std::string &path) {
	std::string fullPath = TextFormat("fonts/%s", font.path.c_str());
	this->font.path = path;

	if (Game::isUsingBin()) {
		if (this->font.path.empty()) {
			this->font.font = Game::getResources().getFont("LanaPixel");
		} else {
			this->font.font = Game::getResources().getFont(GetFileNameWithoutExt(path.c_str()));
		}
	} else {
		if (this->font.path.empty()) {
			auto fontPaths = LoadDirectoryFiles("fonts/");
			if (fontPaths.count > 0) {
				auto fontPath = fontPaths.paths[0];
				fullPath = fontPath;
			}
		}
		this->font.font = LoadFontEx(fullPath.c_str(), this->font.fontSize, nullptr, 256);
	}
}

void TextAreaComponent::loadFont(const std::string &path) {
	std::string fullPath = TextFormat("fonts/%s", font.path.c_str());
	this->font.path = path;

	if (Game::isUsingBin()) {
		if (this->font.path.empty()) {
			this->font.font = Game::getResources().getFont("LanaPixel");
		} else {
			this->font.font = Game::getResources().getFont(GetFileNameWithoutExt(path.c_str()));
		}
	} else {
		if (this->font.path.empty()) {
			auto fontPaths = LoadDirectoryFiles("fonts/");
			if (fontPaths.count > 0) {
				auto fontPath = fontPaths.paths[0];
				fullPath = fontPath;
			}
		}
		this->font.font = LoadFontEx(fullPath.c_str(), this->font.fontSize, nullptr, 256);
	}
}

void ImageRectComponent::scaleImage(int scale) {
	this->image.scale = scale;
	Image image = LoadImage(TextFormat("images/%s", this->image.path.c_str()));
	this->image.scale = (this->image.scale < 1) ? 1 : this->image.scale;
	ImageResizeNN(&image, image.width * this->image.scale, image.height * this->image.scale);
	this->image.texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void ImageRectComponent::loadImage(const std::string &path) {
	this->image.path = path;
	scaleImage(this->image.scale);
}

void NinePatchImageRectComponent::scaleImage(int scale) {
	this->image.scale = scale;
	Image image = LoadImage(TextFormat("images/%s", this->image.path.c_str()));
	this->image.scale = (this->image.scale < 1) ? 1 : this->image.scale;
	ImageResizeNN(&image, image.width * this->image.scale, image.height * this->image.scale);
	this->image.texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void NinePatchImageRectComponent::loadImage(const std::string &path) {
	this->image.path = path;
	scaleImage(this->image.scale);
}

void ButtonComponent::setNormalTextColor(Color color) {
	this->normalTextColor = color;
	this->shownTextColor = color;
}

void DialogueComponent::setDialogue(const DialogueBin &dialogue) {
	dialogueFinished = false;
	finishedTyping = false;

	this->dialogue = dialogue;
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

void DialogueComponent::putChar(Vector2 charMeasure, const char *c, Rectangle rect) {
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

void DialogueComponent::chooseSection(int i, Rectangle rect) {
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

void DialogueComponent::advanceToNextLine() {
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