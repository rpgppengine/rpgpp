#include "system.hpp"

#include <algorithm>

#include "component.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "uiElement.hpp"

void System::update() {
	if (components == nullptr || entitiesManager == nullptr) return;

	for (auto &entity : entities) {
		if (!hasComponent<Rectangle>(entity)) return;

		auto &rect = components->getComponent<Rectangle>(entity);

		if (hasComponent<DialogueComponent>(entity)) {
			auto &dialogue = components->getComponent<DialogueComponent>(entity);

			if (dialogue.line == nullptr) return;

			if (dialogue.dialogue.lines.size() < dialogue.lineIndex) return;
			if (dialogue.line->sections.size() < dialogue.sectionIndex) return;

			dialogue.line = &dialogue.dialogue.lines.at(dialogue.lineIndex);
			dialogue.section = &dialogue.line->sections.at(dialogue.sectionIndex);

			if (dialogue.firstCharTyped == false) {
				dialogue.firstCharTyped = true;
				auto &soundId = dialogue.section->sound;
				Game::getSounds().playSound(soundId);
				return;
			}

			bool finished = false;
			if (dialogue.charIndex == (dialogue.text.size() - 1) || dialogue.charIndex == dialogue.text.size()) {
				finished = true;
			}

			dialogue.finishedTyping = finished;

			// check for delay
			if (dialogue.delay) {
				dialogue.delayDuration -= (GetFrameTime() * static_cast<float>(60.0f / 20.0f));
				if (dialogue.delayDuration <= 0.0f) {
					dialogue.delay = false;
				}
			}

			// advancing char index
			dialogue.frameCounter++;
			if (dialogue.frameCounter > (60 / 20) && !dialogue.delay) {
				dialogue.frameCounter = 0;
				if (dialogue.charIndex < dialogue.text.size()) {
					dialogue.charIndex++;

					// play sound
					if (dialogue.charIndex < dialogue.text.size()) {
						if (dialogue.text.at(dialogue.charIndex) != ' ') {
							auto &soundId = dialogue.section->sound;
							Game::getSounds().playSound(soundId);
						}
					}
				}
			}
		}
	}
}

void System::draw() {
	if (components == nullptr || entitiesManager == nullptr) return;

	for (auto &entity : entities) {
		drawEntity(entity);
	}
}

void System::drawEntity(EntityID entity) {
	if (!hasComponent<VisibilityComponent>(entity)) return;
	if (!hasComponent<Rectangle>(entity)) return;

	auto &visibility = components->getComponent<VisibilityComponent>(entity);
	if (!visibility.isVisible) return;

	auto &rect = components->getComponent<Rectangle>(entity);

	if (hasComponent<ColorRectComponent>(entity)) {
		auto &colorRect = components->getComponent<ColorRectComponent>(entity);

		DrawRectangleRec(rect, colorRect.color);
	}

	if (hasComponent<LabelComponent>(entity)) {
		auto &label = components->getComponent<LabelComponent>(entity);

		Vector2 textSize =
			MeasureTextEx(label.font.font, label.text.c_str(), static_cast<float>(label.font.fontSize), 1);

		Vector2 textPos;
		textPos.x =
			rect.x + Lerp(0.0f, rect.width - textSize.x, (static_cast<float>(label.horizontalAlignment) * 0.5f));
		textPos.y = rect.y + Lerp(0.0f, rect.height - textSize.y, (static_cast<float>(label.verticalAlignment) * 0.5f));

		if (hasComponent<ButtonComponent>(entity)) {
			auto &button = components->getComponent<ButtonComponent>(entity);

			DrawTextEx(label.font.font, label.text.c_str(), textPos, static_cast<float>(label.font.fontSize), 1,
					   button.shownTextColor);
		} else {
			DrawTextEx(label.font.font, label.text.c_str(), textPos, static_cast<float>(label.font.fontSize), 1,
					   label.textColor);
		}
	}

	if (hasComponent<TextAreaComponent>(entity)) {
		auto &textArea = components->getComponent<TextAreaComponent>(entity);

		auto charPos = Vector2{rect.x, rect.y};
		auto charMeasure = Vector2{0, 0};
		for (int i = 0; i < textArea.text.size(); i++) {
			charPos.x += charMeasure.x;

			DrawTextPro(textArea.font.font, TextSubtext(textArea.text.c_str(), i, 1), charPos, Vector2{0, 0}, 0.0f,
						textArea.font.fontSize * 3, 1, BLACK);

			charMeasure = MeasureTextEx(textArea.font.font, TextSubtext(textArea.text.c_str(), i, 1),
										textArea.font.fontSize * 3, 1.0f);
		}
	}

	if (hasComponent<ImageRectComponent>(entity)) {
		auto &imageRect = components->getComponent<ImageRectComponent>(entity);

		DrawTexturePro(imageRect.image.texture,
					   Rectangle{0, 0, static_cast<float>(imageRect.image.texture.width),
								 static_cast<float>(imageRect.image.texture.height)},
					   rect, Vector2{0, 0}, 0.0f, WHITE);
	}

	if (hasComponent<NinePatchImageRectComponent>(entity)) {
		auto &ninePatch = components->getComponent<NinePatchImageRectComponent>(entity);

		Vector2 origin = {0.0f, 0.0f};

		NPatchInfo info = ninePatch.npatchInfo;
		info.source = {0, 0, static_cast<float>(ninePatch.image.texture.width),
					   static_cast<float>(ninePatch.image.texture.height)};
		info.left *= ninePatch.image.scale;
		info.top *= ninePatch.image.scale;
		info.right *= ninePatch.image.scale;
		info.bottom *= ninePatch.image.scale;

		DrawTextureNPatch(ninePatch.image.texture, info, rect, origin, 0.0f, WHITE);
	}

	if (hasComponent<DialogueComponent>(entity)) {
		auto &dialogue = components->getComponent<DialogueComponent>(entity);

		dialogue.textPos = Vector2{0, 0};
		Vector2 charMeasure = Vector2{0, 0};

		for (int i = 0; i < dialogue.charIndex; i++) {
			dialogue.chooseSection(i, rect);

			if (dialogue.maxLineHeight < charMeasure.y) {
				dialogue.maxLineHeight = charMeasure.y;
			}

			if (dialogue.font.path != dialogue.section->font) {
				dialogue.font.font = Game::getResources().getFont(dialogue.section->font);
				dialogue.font.path = dialogue.section->font;
			}

			if (dialogue.section->key == "delay" || dialogue.section->delay > 0) {
				if (!dialogue.delay) {
					dialogue.delay = true;
					dialogue.delayDuration = dialogue.section->delay;
				}
			}

			if (!dialogue.appliedTag) {
				if (dialogue.section->padding > 0.0f) {
					charMeasure.x += dialogue.padding;
					dialogue.appliedTag = true;
				}
			}

			// draw the character
			const char *subText = TextSubtext(dialogue.text.c_str(), i, 1);
			dialogue.putChar(charMeasure, subText, rect);

			Vector2 newMeasure =
				MeasureTextEx(Game::getResources().getFont(dialogue.section->font),
							  TextSubtext(dialogue.text.c_str(), i, 1), dialogue.section->textSize * 3, 1.0f);
			charMeasure = newMeasure;
		}
	}
}

void System::onNotify(Event event, EntityID entity) {
	auto &input = components->getComponent<InputComponent>(entity);

	if (event.key == KEY_UP) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.upButton.title);
	}
	if (event.key == KEY_DOWN) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.downButton.title);
	}
	if (event.key == KEY_LEFT) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.leftButton.title);
	}
	if (event.key == KEY_RIGHT) {
		Game::getUi().getCurrentView()->changeFocusedElement(input.rightButton.title);
	}
	if (event.key == KEY_Z) {
		input.callbacks[CALLBACK_TRIGGER]();
	}
}
