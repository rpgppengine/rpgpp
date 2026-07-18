#include "interfaceService.hpp"

#include <raylib.h>

#include <memory>
#include <stdexcept>

#include "dialogueBalloon.hpp"
#include "dialogueInterfaceView.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "sol/forward.hpp"
#include "sol/types.hpp"

ElementFactory InterfaceService::factory = {};

InterfaceService::InterfaceService() {
	this->frameCounter = 0;
	this->transitionActive = false;
	this->transitionColor = Color{0, 0, 0, 1};
	this->alpha = 0.0f;
	this->transitionSecondStage = false;

	fpsVisible = false;

	TweenProvider::setupTweenFuncs();

	this->font = Game::getResources().getFont("LanaPixel");

	Rectangle destRec = Rectangle{0, 0, static_cast<float>(GetScreenWidth() - 20), 140};
	destRec.x = (GetScreenWidth() - destRec.width) / 2;
	destRec.y = (GetScreenHeight() - destRec.height) - 20;

	this->views = std::map<std::string, std::unique_ptr<InterfaceView>>{};

	auto diagView = std::make_unique<DialogueInterfaceView>();
	views["dialogue"] = std::move(diagView);
}

InterfaceService::~InterfaceService() {
	// UnloadFont(font);
	// UnloadTexture(this->uiTexture);
}

void InterfaceService::initBin(GameData &bin) {
	if (bin.interfaceViews.count("dialogue") > 0) {
		auto diagView = std::make_unique<DialogueInterfaceView>(bin.interfaceViews["dialogue"]);
		views["dialogue"] = std::move(diagView);
	}

	for (auto &[title, viewBin] : bin.interfaceViews) {
		if (title != "dialogue") {
			auto view = std::make_unique<InterfaceView>(viewBin);
			views.emplace(title, std::move(view));
		}
	}
}

Font InterfaceService::getFont() const { return font; }

void InterfaceService::showDialogue(const std::string &id, bool runScript) {
	if (Game::getBin().dialogues.count(id) > 0) {
		auto diag = Game::getBin().dialogues[id];
		showDialogue(diag, runScript);
	} else {
		throw std::runtime_error(TextFormat("This Dialogue does not exist: %s", id.c_str()));
	}
}

void InterfaceService::showDialogue(const DialogueBin &dialogue, bool runScript) {
	if (currentViewName != "dialogue") {
		showInterface("dialogue", runScript);
		DialogueInterfaceView *diagView = static_cast<DialogueInterfaceView *>(views.at("dialogue").get());
		diagView->setDialogue(dialogue);
	}
}

InterfaceView *InterfaceService::getCurrentView() {
	if (views.count(currentViewName) > 0) {
		return views.at(currentViewName).get();
	} else {
		return nullptr;
	}
}

void InterfaceService::showInterface(const std::string &title, bool runScript) {
	if (views.count(title) > 0) {
		currentViewName = title;
		notifyLock = true;

		auto &worldServiceRef = Game::getWorld();

		if (worldServiceRef.getIfRoomExist()) {
			worldServiceRef.getPlayer().setMovementLock(true);
		}

		views[title]->runScript = runScript;

		auto &env = views[title]->getLuaEnvironment();
		if (views[title]->hasScript()) {
			if (env["open"].is<sol::function>() && runScript) {
				env["open"]();
			}
		}
	}
}

void InterfaceService::hideInterface(bool runScript) {
	if (views.count(currentViewName) > 0) {
		auto &env = views[currentViewName]->getLuaEnvironment();
		if (views[currentViewName]->hasScript()) {
			if (env["close"].is<sol::function>() && runScript) {
				env["close"]();
			}
		}

		currentViewName = "";
		Game::getWorld().getPlayer().setMovementLock(false);
	}
}

ElementFactory &InterfaceService::getFactory() { return factory; }

void InterfaceService::doFadeTransition() {
	this->transitionActive = true;
	this->frameCounter = 0;
	this->transitionColor = Color{0, 0, 0, 1};
	this->alpha = 0.0f;
	this->transitionSecondStage = false;
}

void InterfaceService::setNotifyLock() { notifyLock = true; }

bool InterfaceService::getNotifyLock() { return notifyLock; }

KeyboardKey InterfaceService::getLastKey() { return lastKey; }

void InterfaceService::updateFade() {
	if (transitionActive) {
		frameCounter++;
		if (frameCounter >= 2) {
			frameCounter = 0;
			if (!transitionSecondStage) {
				if (alpha < 1.0f) {
					alpha += 0.02f;
					transitionColor = Fade(transitionColor, alpha);
				} else {
					transitionSecondStage = true;

					// halfpoint callback
					onTransitionPoint();
					onTransitionPoint = nullptr;
				}
			} else {
				if (alpha > 0.0f) {
					alpha -= 0.02f;
					transitionColor = Fade(transitionColor, alpha);
				} else {
					transitionActive = false;
					transitionSecondStage = false;
				}
			}
		}
	}
}

void InterfaceService::setTransitionPointCallback(std::function<void()> callback) {
	this->onTransitionPoint = callback;
}

void InterfaceService::update() {
	updateFade();

	if (IsKeyPressed(KEY_Q)) {
		fpsVisible = !fpsVisible;
	}

	if (views.count(currentViewName) > 0) {
		if (!notifyLock) {
			auto key = GetKeyPressed();
			if (key != KEY_NULL) {
				lastKey = static_cast<KeyboardKey>(key);
				views.at(currentViewName)->onNotify({static_cast<KeyboardKey>(key)});
			}

			if (lastKey != KEY_NULL) {
				if (IsKeyDown(lastKey)) {
					views.at(currentViewName)->onNotify({static_cast<KeyboardKey>(lastKey), true});
				}
			}
		}
		views.at(currentViewName)->update();
	}

	for (auto &&item : views) {
		item.second->update();
	}

	if (notifyLock) {
		notifyLock = false;
	}
}

void InterfaceService::draw() {
	if (fpsVisible) {
		DrawFPS(10, 10);
		DrawTextEx(font, "rpgpp", Vector2{10, 36}, static_cast<float>(font.baseSize), 2, RED);
	}

	if (views.count(currentViewName) > 0) {
		views.at(currentViewName)->draw();
	}

	if (transitionActive) {
		DrawRectangleRec(Rectangle{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
						 transitionColor);
	}
}

void InterfaceService::unload() const {
	// Abandon sol::environments before ScriptService is destructed
	for (auto &[title, view] : views) {
		view->abandonLua();
	}
}
