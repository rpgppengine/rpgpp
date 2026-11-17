#include "game.hpp"

#include <raylib.h>
#include <tinyfiledialogs.h>

#include <memory>
#include <sol/forward.hpp>
#include <sol/sol.hpp>
#include <stdexcept>
#include <string>
#include <system_error>

#include "gamedata.hpp"
#include "resourceUnpacker.hpp"
#include "scriptService.hpp"
#include "soundService.hpp"

Game *Game::instance_ = nullptr;
std::unique_ptr<GameData> Game::gameData = std::unique_ptr<GameData>{};
bool Game::usesBin = false;
bool Game::willClose = false;
std::unique_ptr<StateService> Game::state = std::unique_ptr<StateService>{};
std::unique_ptr<WorldService> Game::world = std::unique_ptr<WorldService>{};
std::unique_ptr<InterfaceService> Game::ui = std::unique_ptr<InterfaceService>{};
std::unique_ptr<ResourceService> Game::resources = std::unique_ptr<ResourceService>{};
std::unique_ptr<SoundService> Game::sounds = std::unique_ptr<SoundService>{};
std::unique_ptr<ScriptService> Game::scripts = std::unique_ptr<ScriptService>{};

Game::Game() {
	if (instance_ == nullptr) {
		instance_ = this;
	} else {
		throw std::runtime_error("Instance of Game already exists.");
	}
}

Game &Game::instance() {
	if (instance_ == nullptr) {
		throw std::runtime_error("There is no instance of Game.");
	}

	return *instance_;
}

Game *Game::instancePtr() { return instance_; }

void Game::init() {
	gameData = std::make_unique<GameData>();

	resources = std::make_unique<ResourceService>();
	state = std::make_unique<StateService>();
	world = std::make_unique<WorldService>();
	ui = std::make_unique<InterfaceService>();
	sounds = std::make_unique<SoundService>();
	scripts = std::make_unique<ScriptService>();
}

bool Game::isUsingBin() { return usesBin; }

void Game::useBin(const std::string &filePath) {
	if (!FileExists(filePath.c_str())) {
		tinyfd_messageBox("Error!", "The packed resources (game.rres) does not exist!", "ok", "error", 1);
		return;
	}

	usesBin = true;

	auto &unpacker = resources->getUnpacker();
	std::error_code ec;
	unpacker.mapFile(filePath, ec);
	auto gameData = unpacker.getMinData();

	SetWindowTitle(gameData.title.c_str());
	SetWindowSize(gameData.programSet.windowSize.x, gameData.programSet.windowSize.y);
	if (gameData.programSet.windowResizeableFlag) {
		SetWindowState(FLAG_WINDOW_RESIZABLE);
	} else {
		ClearWindowState(FLAG_WINDOW_RESIZABLE);
	}
	SetWindowState(gameData.programSet.windowStateFlag);
	if (gameData.programSet.targetFPS > 0) {
		SetTargetFPS(gameData.programSet.targetFPS);
	}

	auto iconImg = unpacker.loadImage(gameData.programSet.programIconPath);
	if (IsImageValid(iconImg)) {
		SetWindowIcon(iconImg);
		UnloadImage(iconImg);
	}

	ui->init();

	if (!gameData.gameSet.defaultLoadingPath.empty()) {
		if (!gameData.gameSet.isLoadUi) {
			world->setRoomBin(gameData.gameSet.defaultLoadingPath);
		} else {
			ui->showInterface(gameData.gameSet.defaultLoadingPath);
		}
	}
}

GameData &Game::getBin() { return *gameData; }

ResourceService &Game::getResources() { return *resources; }

StateService &Game::getState() { return *state; }

WorldService &Game::getWorld() { return *world; }

InterfaceService &Game::getUi() { return *ui; }

SoundService &Game::getSounds() { return *sounds; }

ScriptService &Game::getScripts() { return *scripts; }

void Game::update() {
	if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) willClose = true;

	sounds->update();
	world->update();
	ui->update();
}

void Game::draw() {
	world->draw();
	ui->draw();
}

void Game::unload() {
	sounds->unload();
	world->unload();
	ui->unload();
	// NOTE: Without this, we might get segfaults... beware!
	state.reset();

	resources->unload();
}

void Game::setLua(sol::state_view lua) { scripts->setLua(lua); }

void Game::closeGame() { willClose = true; }

bool Game::windowWillClose() { return willClose; }
