#include "dialogueInterfaceView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "button.hpp"
#include "component.hpp"
#include "dialogueArea.hpp"
#include "dialogueBalloon.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "imageRect.hpp"
#include "interfaceElementFactory.hpp"
#include "interfaceView.hpp"
#include "ninePatchImageRect.hpp"
#include "raylib.h"
#include "uiElement.hpp"

static const Rectangle diagImageRect = Rectangle{10, 320, 620, 140};
static const Rectangle normalDiagAreaRect = Rectangle{19, 329, 611, 131};
static const Rectangle portraitDiagAreaRect =
	Rectangle{normalDiagAreaRect.x + diagImageRect.height, normalDiagAreaRect.y,
			  normalDiagAreaRect.width - diagImageRect.height - (9 * 2), normalDiagAreaRect.height};
static const Rectangle optionsRect =
	Rectangle{diagImageRect.x + (diagImageRect.width - 180), diagImageRect.y - 8 - 180, 180, 180};

DialogueInterfaceView::DialogueInterfaceView() : InterfaceView(Rectangle{}) {
	auto diagImage = construct("diagImage", "NPatchImage", ecs);
	ecs.getComponent<Rectangle>(diagImage) = diagImageRect;
	auto &diagImageComponent = ecs.getComponent<NinePatchImageRectComponent>(diagImage);
	diagImageComponent.npatchInfo.top = 3;
	diagImageComponent.npatchInfo.left = 3;
	diagImageComponent.npatchInfo.bottom = 3;
	diagImageComponent.npatchInfo.right = 3;
	diagImageComponent.npatchInfo.layout = NPATCH_NINE_PATCH;
	diagImageComponent.scaleImage(3);
	diagImageComponent.loadImage("ui-npatch.png");

	auto diagArea = construct("dialogueArea", "Dialogue", ecs);
	ecs.getComponent<Rectangle>(diagArea) = normalDiagAreaRect;

	auto portrait = construct("portrait", "Image", ecs);
	ecs.getComponent<VisibilityComponent>(portrait).isVisible = false;
	ecs.getComponent<Rectangle>(portrait) = {diagImageRect.x + 9, diagImageRect.y + 9, diagImageRect.height - (9 * 2),
											 diagImageRect.height - (9 * 2)};

	auto optionsImage = construct("optionsImage", "NPatchImage", ecs);
	ecs.getComponent<VisibilityComponent>(optionsImage).isVisible = false;
	ecs.getComponent<Rectangle>(optionsImage) = optionsRect;
	auto &optionsImageComponent = ecs.getComponent<NinePatchImageRectComponent>(optionsImage);
	optionsImageComponent.npatchInfo.top = 3;
	optionsImageComponent.npatchInfo.left = 3;
	optionsImageComponent.npatchInfo.bottom = 3;
	optionsImageComponent.npatchInfo.right = 3;
	optionsImageComponent.npatchInfo.layout = NPATCH_NINE_PATCH;
	optionsImageComponent.scaleImage(3);
	optionsImageComponent.loadImage("ui-npatch.png");

	auto optionButton = construct("optionButton", "Button", ecs);
	ecs.getComponent<VisibilityComponent>(optionButton).isVisible = false;
	ecs.getComponent<Rectangle>(optionButton) = {optionsRect.x + 9, optionsRect.y + 9, optionsRect.width - (9 * 2), 51};
	ecs.getComponent<ButtonComponent>(optionButton).setNormalTextColor(WHITE);
	ecs.getComponent<ButtonComponent>(optionButton).focusedTextColor = YELLOW;
	ecs.getComponent<ColorRectComponent>(optionButton).color = BLANK;
	ecs.getComponent<LabelComponent>(optionButton).text = "Test.";
	ecs.getComponent<LabelComponent>(optionButton).horizontalAlignment = TEXT_ALIGN_CENTRE;
	ecs.getComponent<LabelComponent>(optionButton).verticalAlignment = TEXT_ALIGN_MIDDLE;
	ecs.getComponent<LabelComponent>(optionButton).font.fontSize = 13 * 3;
	initEntityComponents(optionButton);

	// test dialogue
	DialogueBin testDialogue;
	DialogueLine diagLine;
	diagLine.sections.push_back({"", "Hello!"});
	testDialogue.lines.push_back(diagLine);
	ecs.getComponent<DialogueComponent>(diagArea).setDialogue(testDialogue);
}

DialogueInterfaceView::DialogueInterfaceView(const std::string &filePath) : InterfaceView(filePath) {}

DialogueInterfaceView::DialogueInterfaceView(InterfaceViewBin &bin) : InterfaceView(bin) {}

void DialogueInterfaceView::setDialogue(DialogueBin dialogue) {
	this->dialogue = dialogue;

	auto diagArea = ecs.getEntityManager().findName("dialogueArea");
	if (diagArea != MAX_ENTITIES) {
		ecs.getComponent<DialogueComponent>(diagArea).setDialogue(dialogue);
	}
}

void DialogueInterfaceView::onNotify(Event event) {
	InterfaceView::onNotify(event);

	if (event.key == KEY_Z) {
		auto diagArea = ecs.getEntityManager().findName("dialogueArea");
		if (diagArea != MAX_ENTITIES) {
			auto &diagComponent = ecs.getComponent<DialogueComponent>(diagArea);
			if (!Game::getUi().getNotifyLock()) {
				diagComponent.advanceToNextLine();
			}

			if (diagComponent.dialogueFinished) return;

			auto portrait = ecs.getEntityManager().findName("portrait");
			if (diagComponent.line->hasPortrait) {
				ecs.getComponent<ImageRectComponent>(portrait).loadImage(diagComponent.line->imageId);
				ecs.getComponent<VisibilityComponent>(portrait).isVisible = true;
				ecs.getComponent<Rectangle>(diagArea) = portraitDiagAreaRect;
			} else {
				ecs.getComponent<VisibilityComponent>(portrait).isVisible = false;
				ecs.getComponent<Rectangle>(diagArea) = normalDiagAreaRect;
			}

			auto optionButton = ecs.getEntityManager().findName("optionButton");
			if (diagComponent.line->hasOptions) {
				Rectangle originRect = ecs.getComponent<Rectangle>(optionButton);
				auto buttonBaseJson = dumpEntityJson(optionButton);
				int i = 0;
				for (auto &option : diagComponent.line->options) {
					auto newButton = ecs.createEntity(TextFormat("option-%i", i));
					for (auto &componentJson : buttonBaseJson.items()) {
						ecs.insertComponentFromJson(newButton, componentJson.key(), componentJson.value());
					}

					ecs.getComponent<Rectangle>(newButton) = {originRect.x, originRect.y + (originRect.height * i),
															  originRect.width, originRect.height};
					ecs.getComponent<VisibilityComponent>(newButton).isVisible = false;
					ecs.getComponent<LabelComponent>(newButton).text = option.title;

					ecs.getComponent<InputComponent>(newButton).upButton.title = TextFormat("option-%i", i - 1);
					ecs.getComponent<InputComponent>(newButton).downButton.title = TextFormat("option-%i", i + 1);

					initEntityComponents(newButton);

					ecs.getComponent<InputComponent>(newButton).callbacks[CALLBACK_TRIGGER] = [this, &option] {
						if (Game::isUsingBin()) {
							Game::getUi().hideInterface();
							Game::getUi().showDialogue(option.nextDialogue);
							Game::getUi().setNotifyLock();
							if (optionsCount > 0) {
								for (int i = 0; i < optionsCount; i++) {
									std::string elementName = TextFormat("option-%i", i);
									auto buttonEntity = ecs.getEntityManager().findName(elementName);
									if (buttonEntity != MAX_ENTITIES) {
										ecs.destroyEntity(buttonEntity);
									}
								}
							}
						}
					};

					i++;
				}

				changeFocusedElement("option-0");
				optionsCount = i + 1;
			} else {
				auto optionsImage = ecs.getEntityManager().findName("optionsImage");
				ecs.getComponent<VisibilityComponent>(optionsImage).isVisible = false;
				for (int i = 0; i < optionsCount; i++) {
					std::string elementName = TextFormat("option-%i", i);
					auto entity = ecs.getEntityManager().findName(elementName);
					if (entity != MAX_ENTITIES) {
						ecs.destroyEntity(entity);
					}
				}
				optionsCount = 0;
				current = MAX_ENTITIES;
			}
		}
	}
}

void DialogueInterfaceView::update() {
	auto diagArea = ecs.getEntityManager().findName("dialogueArea");
	auto optionsImage = ecs.getEntityManager().findName("optionsImage");
	if (diagArea != MAX_ENTITIES) {
		auto &diagAreaComponent = ecs.getComponent<DialogueComponent>(diagArea);
		if (diagAreaComponent.finishedTyping && diagAreaComponent.line->hasOptions) {
			ecs.getComponent<VisibilityComponent>(optionsImage).isVisible = true;

			int i = 0;
			for (auto &option : diagAreaComponent.line->options) {
				auto optionEntity = ecs.getEntityManager().findName(TextFormat("option-%i", i));
				ecs.getComponent<VisibilityComponent>(optionEntity).isVisible = true;
				i++;
			}
		}
		if (diagAreaComponent.dialogueFinished) {
			Game::getUi().hideInterface();
		}
	}

	InterfaceView::update();
}

void DialogueInterfaceView::draw() { InterfaceView::draw(); }