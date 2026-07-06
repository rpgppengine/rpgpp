#include "dialogueInterfaceView.hpp"

#include <cstdio>
#include <memory>
#include <vector>

#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "ui_elements/button.hpp"
#include "ui_elements/dialogueArea.hpp"
#include "ui_elements/imageRect.hpp"
#include "ui_elements/ninePatchImageRect.hpp"

static const Rectangle diagImageRect = Rectangle{10, 320, 620, 140};
static const Rectangle normalDiagAreaRect = Rectangle{19, 329, 611, 131};
static const Rectangle portraitDiagAreaRect =
	Rectangle{normalDiagAreaRect.x + diagImageRect.height, normalDiagAreaRect.y,
			  normalDiagAreaRect.width - diagImageRect.height - (9 * 2), normalDiagAreaRect.height};
static const Rectangle optionsRect =
	Rectangle{diagImageRect.x + (diagImageRect.width - 180), diagImageRect.y - 8 - 180, 180, 180};

DialogueInterfaceView::DialogueInterfaceView() : InterfaceView(Rectangle{}) {}

DialogueInterfaceView::DialogueInterfaceView(const std::string &filePath) : InterfaceView(filePath) {}

DialogueInterfaceView::DialogueInterfaceView(InterfaceViewBin &bin) : InterfaceView(bin) {}

void DialogueInterfaceView::setDialogue(DialogueBin dialogue) {
	this->dialogue = dialogue;

	DialogueArea *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
	if (diagArea != nullptr) {
		diagArea->setDialogue(dialogue);
	}
}

void DialogueInterfaceView::onNotify(Event event) {
	InterfaceView::onNotify(event);

	if (event.key == KEY_Z) {
		DialogueArea *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));

		if (diagArea != nullptr) {
			if (!Game::getUi().getNotifyLock()) {
				diagArea->advanceToNextLine();
			}

			if (diagArea->dialogueFinished) return;

			ImageRect *portrait = static_cast<ImageRect *>(getElement("portrait"));
			if (diagArea->line->hasPortrait) {
				portrait->loadImage(diagArea->line->imageId);
				portrait->props["visible"] = true;
				diagArea->props["rect"] = portraitDiagAreaRect;
			} else {
				portrait->props["visible"] = false;
				diagArea->props["rect"] = normalDiagAreaRect;
			}

			Button *optionButton = static_cast<Button *>(getElement("optionButton"));
			if (diagArea->line->hasOptions) {
				Rectangle originRect = std::get<Rectangle>(optionButton->props["rect"]);

				int i = 0;
				for (auto &option : diagArea->line->options) {
					Button *newButton = static_cast<Button *>(cloneElement("optionButton", TextFormat("option-%i", i)));

					Rectangle &newRect = std::get<Rectangle>(newButton->props["rect"]);
					newRect = {originRect.x, originRect.y + (originRect.height * i), originRect.width,
							   originRect.height};
					newButton->props["visible"] = false;
					newButton->props["text"] = option.title;

					InputC &input = std::get<InputC>(newButton->props["input"]);
					input.upButton.title = TextFormat("option-%i", i - 1);
					input.downButton.title = TextFormat("option-%i", i + 1);

					newButton->callbacks[CALLBACK_TRIGGER] = [this, &option] {
						if (Game::isUsingBin()) {
							Game::getUi().hideInterface(false);
							Game::getUi().showDialogue(option.nextDialogue, false);
							Game::getUi().setNotifyLock();
							if (optionsCount > 0) {
								for (int i = 0; i < optionsCount; i++) {
									std::string elementName = TextFormat("option-%i", i);
									removeElement(elementName);
								}
							}
						}
					};

					i++;
				}

				changeFocusedElement("option-0");
				optionsCount = i + 1;
				// has options
			} else {
				NinePatchImageRect *optionsImage = static_cast<NinePatchImageRect *>(getElement("optionsImage"));
				optionsImage->props["visible"] = false;
				for (int i = 0; i < optionsCount; i++) {
					std::string elementName = TextFormat("option-%i", i);
					removeElement(elementName);
				}
				optionsCount = 0;
				currentElement = MAX_ELEMENTS;
			}
		}
	}
}

void DialogueInterfaceView::update() {
	DialogueArea *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
	NinePatchImageRect *optionsImage = static_cast<NinePatchImageRect *>(getElement("optionsImage"));
	if (diagArea != nullptr) {
		if (diagArea->finishedTyping) {
			if (!notifiedEndLine) {
				// run user-defined function if any
				if (hasScript()) {
					if (env["on_line_finished"].is<sol::function>()) {
						env["on_line_finished"]();
					}
				}

				notifiedEndLine = true;
			}

			// show options if any
			if (diagArea->line->hasOptions) {
				optionsImage->props["visible"] = true;

				int i = 0;
				for (auto& option : diagArea->line->options) {
					Button* optionButton = static_cast<Button*>(getElement(TextFormat("option-%i", i)));
					optionButton->props["visible"] = true;

					i++;
				}
			}
		} else {
			notifiedEndLine = false;
		}

		if (diagArea->dialogueFinished) {
			Game::getUi().hideInterface();
		}
	}

	InterfaceView::update();
}

void DialogueInterfaceView::draw() { InterfaceView::draw(); }
