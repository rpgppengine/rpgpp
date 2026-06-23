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

InterfaceService::InterfaceService() {
	fpsVisible = false;

	this->font = Game::getResources().getFont("LanaPixel");

	Image img = LoadImage("images/ui-npatch.png");
	ImageResizeNN(&img, img.width * 3, img.height * 3);

	this->uiTexture = LoadTextureFromImage(img);

	Rectangle destRec = Rectangle{0, 0, static_cast<float>(GetScreenWidth() - 20), 140};
	destRec.x = (GetScreenWidth() - destRec.width) / 2;
	destRec.y = (GetScreenHeight() - destRec.height) - 20;
	this->dialogue = DialogueBalloon(destRec);

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
		if (title == "dialogue") return;

		auto view = std::make_unique<InterfaceView>(viewBin);
		views.emplace(title, std::move(view));
	}
}

Font InterfaceService::getFont() const { return font; }

Texture InterfaceService::getTexture() const { return uiTexture; }

void InterfaceService::showDialogue(const std::string &id) {
	if (Game::getBin().dialogues.count(id) > 0) {
		auto diag = Game::getBin().dialogues[id];
		showDialogue(diag);
	} else {
		throw std::runtime_error(TextFormat("This Dialogue does not exist: %s", id.c_str()));
	}
}

void InterfaceService::showDialogue(const DialogueBin &dialogue) {
	if (currentViewName != "dialogue") {
		DialogueInterfaceView *diagView = static_cast<DialogueInterfaceView *>(views.at("dialogue").get());
		diagView->setDialogue(dialogue);
		showInterface("dialogue");
	}
}

InterfaceView *InterfaceService::getCurrentView() {
	if (views.count(currentViewName) > 0) {
		return views.at(currentViewName).get();
	} else {
		return nullptr;
	}
}

void InterfaceService::showInterface(const std::string &title) {
	if (views.count(title) > 0) {
		currentViewName = title;
		notifyLock = true;
		Game::getWorld().getPlayer().setMovementLock(true);

		auto &env = views[title]->getLuaEnvironment();
		if (env["open"].is<sol::function>()) {
			env["open"]();
		}
	}
}

void InterfaceService::hideInterface() {
	if (views.count(currentViewName) > 0) {
		auto &env = views[currentViewName]->getLuaEnvironment();
		if (env["close"].is<sol::function>()) {
			env["close"]();
		}

		currentViewName = "";
		Game::getWorld().getPlayer().setMovementLock(false);
	}
}

void InterfaceService::setNotifyLock() { notifyLock = true; }

bool InterfaceService::getNotifyLock() { return notifyLock; }

void InterfaceService::update() {
	if (IsKeyPressed(KEY_Q)) {
		fpsVisible = !fpsVisible;
	}

	if (views.count(currentViewName) > 0) {
		if (!notifyLock) {
			auto key = GetKeyPressed();
			if (key != KEY_NULL) {
				views.at(currentViewName)->onNotify({static_cast<KeyboardKey>(key)});
			}
		}
		views.at(currentViewName)->update();
	}

	dialogue.update();

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

	dialogue.draw();

	if (views.count(currentViewName) > 0) {
		views.at(currentViewName)->draw();
	}
}

void InterfaceService::unload() const {
	UnloadTexture(this->uiTexture);

	// Abandon sol::environments before ScriptService is destructed
	for (auto &[title, view] : views) {
		view->getLuaEnvironment().abandon();
	}
}
