#include "dialogueInterfaceView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "button.hpp"
#include "dialogueArea.hpp"
#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "imageRect.hpp"
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
	NinePatchImageRect *diagImage = new NinePatchImageRect(diagImageRect);
	diagImage->npatchInfo.top = 3;
	diagImage->npatchInfo.left = 3;
	diagImage->npatchInfo.bottom = 3;
	diagImage->npatchInfo.right = 3;
	diagImage->npatchInfo.layout = NPATCH_NINE_PATCH;
	diagImage->setScale(3);
	diagImage->setSource("ui-npatch.png");

	addElement("diagImage", diagImage, 0);

	DialogueBin testDialogue;
	DialogueLine diagLine;
	diagLine.sections.push_back({"", "Hello!"});
	testDialogue.lines.push_back(diagLine);

	DialogueArea *diagArea = new DialogueArea(normalDiagAreaRect);
	diagArea->setRect(normalDiagAreaRect);
	diagArea->setDialogue(testDialogue);

	addElement("dialogueArea", diagArea, 1);

	ImageRect *portraitRect = new ImageRect(Rectangle{diagImageRect.x + 9, diagImageRect.y + 9,
													  diagImageRect.height - (9 * 2), diagImageRect.height - (9 * 2)});
	portraitRect->setVisible(false);
	addElement("portrait", portraitRect, 1);

	// options

	NinePatchImageRect *optionsImage = new NinePatchImageRect(optionsRect);
	optionsImage->npatchInfo.top = 3;
	optionsImage->npatchInfo.left = 3;
	optionsImage->npatchInfo.bottom = 3;
	optionsImage->npatchInfo.right = 3;
	optionsImage->npatchInfo.layout = NPATCH_NINE_PATCH;
	optionsImage->setScale(3);
	optionsImage->setSource("ui-npatch.png");

	optionsImage->setVisible(false);

	addElement("optionsImage", optionsImage, 0);

	Button *optionButton = new Button(Rectangle{optionsRect.x + 9, optionsRect.y + 9, optionsRect.width - (9 * 2), 51});
	optionButton->setVisible(false);
	optionButton->setBackgroundColor(BLANK);
	optionButton->setNormalTextColor(WHITE);
	optionButton->setFocusedTextColor(YELLOW);
	optionButton->setTextSize(13 * 3);

	addElement("optionButton", optionButton, 1);

	auto dump = dumpJson();
	auto str = dump.dump();
	SaveFileText("dump.json", str.c_str());
}

DialogueInterfaceView::DialogueInterfaceView(const std::string &filePath) : InterfaceView(filePath) {}

DialogueInterfaceView::DialogueInterfaceView(InterfaceViewBin &bin) : InterfaceView(bin) {}

void DialogueInterfaceView::setDialogue(DialogueBin dialogue) {
	this->dialogue = dialogue;
	if (elementExists("dialogueArea")) {
		auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
		diagArea->setDialogue(dialogue);
	}
}

void DialogueInterfaceView::onNotify(Event event) {
	InterfaceView::onNotify(event);

	if (elementExists(focusedElementName)) return;

	if (event.key == KEY_Z) {
		if (elementExists("dialogueArea")) {
			printf("a \n");
			auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
			if (!Game::getUi().getNotifyLock()) {
				diagArea->advanceToNextLine();
			}

			if (diagArea->isDialogueFinished()) return;

			if (diagArea->line->hasPortrait) {
				auto *portrait = static_cast<ImageRect *>(getElement("portrait"));
				portrait->setSource(diagArea->line->imageId);
				portrait->setVisible(true);
				diagArea->setRect(portraitDiagAreaRect);
			} else {
				getElement("portrait")->setVisible(false);
				diagArea->setRect(normalDiagAreaRect);
			}

			if (diagArea->line->hasOptions && elementExists("optionButton")) {
				Button *baseButton = static_cast<Button *>(getElement("optionButton"));
				Rectangle originRect = baseButton->getRect();
				auto buttonProps = baseButton->dumpJson();
				int i = 0;
				for (auto &option : diagArea->line->options) {
					Button *button = new Button();
					button->fromJson(buttonProps);
					button->setVisible(false);
					button->setText(option.title);

					Rectangle buttonRect = {originRect.x, originRect.y + (originRect.height * i), originRect.width,
											originRect.height};
					button->setRect(buttonRect);

					button->setUpElement(TextFormat("option-%i", i - 1));
					button->setDownElement(TextFormat("option-%i", i + 1));

					button->setCallback(CALLBACK_TRIGGER, [this, &option] {
						if (Game::isUsingBin()) {
							printf("b \n");
							Game::getUi().hideInterface();
							Game::getUi().showDialogue(option.nextDialogue);
							Game::getUi().setNotifyLock();
							if (optionsCount > 0) {
								for (int i = 0; i < optionsCount; i++) {
									std::string elementName = TextFormat("option-%i", i);
									if (elementExists(elementName)) {
										removeElement(elementName);
									}
								}
							}
						}
					});

					addElement(TextFormat("option-%i", i), button, 2);

					i++;
				}
				changeFocusedElement("option-0");
				optionsCount = i + 1;
			} else {
				getElement("optionsImage")->setVisible(false);
				if (optionsCount > 0) {
					for (int i = 0; i < optionsCount; i++) {
						std::string elementName = TextFormat("option-%i", i);
						if (elementExists(elementName)) {
							removeElement(elementName);
						}
					}
				}
				optionsCount = 0;
			}
		}
	}
}

void DialogueInterfaceView::update() {
	if (elementExists("dialogueArea")) {
		auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
		if (diagArea->isFinishedTyping() && diagArea->line->hasOptions) {
			getElement("optionsImage")->setVisible(true);
			int i = 0;
			for (auto &option : diagArea->line->options) {
				getElement(TextFormat("option-%i", i))->setVisible(true);
				i++;
			}
		}
		if (diagArea->isDialogueFinished()) {
			// finished dialogue..
			Game::getUi().hideInterface();
		}
	}

	InterfaceView::update();
}

void DialogueInterfaceView::draw() { InterfaceView::draw(); }