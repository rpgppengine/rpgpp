#include "game.hpp"

#include <raylib.h>

#include <memory>
#include <sol/forward.hpp>
#include <sol/sol.hpp>	// FIXME : lua.h not found
#include <stdexcept>
#include <string>

#include "gamedata.hpp"
#include "scriptService.hpp"
#include "soundService.hpp"

Game *Game::instance_ = nullptr;
std::unique_ptr<GameData> Game::gameData = std::unique_ptr<GameData>{};
bool Game::usesBin = false;
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

void Game::init(bool usesBin) {
	gameData = std::make_unique<GameData>();
	Game::usesBin = false;

	if (usesBin) {
		std::string filePath = "game.bin";
		gameData = std::make_unique<GameData>(deserializeFile(filePath));
		Game::usesBin = true;
	}

	resources = std::make_unique<ResourceService>();
	state = std::make_unique<StateService>();
	world = std::make_unique<WorldService>();
	ui = std::make_unique<InterfaceService>();
	sounds = std::make_unique<SoundService>();
	scripts = std::make_unique<ScriptService>();
}

bool Game::isUsingBin() { return usesBin; }

void Game::useBin(const std::string &filePath) {
	gameData = std::make_unique<GameData>(deserializeFile(filePath));
	usesBin = true;

	// resources
	// resources->init();

	/// Setup program
	SetWindowTitle(gameData->title.c_str());
	SetWindowSize(gameData->programSet.windowSize.x, gameData->programSet.windowSize.y);
	if (gameData->programSet.windowResizeableFlag) {
		SetWindowState(FLAG_WINDOW_RESIZABLE);
	} else {
		ClearWindowState(FLAG_WINDOW_RESIZABLE);
	}
	SetWindowState(gameData->programSet.windowStateFlag);
	if (gameData->programSet.targetFPS > 0) {
		SetTargetFPS(gameData->programSet.targetFPS);
	}

	auto iconTexture = resources->getTexture(GetFileName(gameData->programSet.programIconPath.c_str()));
	Image iconImage = LoadImageFromTexture(iconTexture);

	SetWindowIcon(iconImage);

	UnloadImage(iconImage);

	/// Setup Interfaces
	ui->initBin(*gameData);

	/// Select the default room from the settings

	// Thefirey33:
	// Decided on a very hybrid system that allows the user to load a Room or an UIElement at first.
	// Had an idea where some games might want the player to immediately start without a main menu, as some RPG games do want to do that.

	// NOTE: Hacky solution. Will fix later when i'm not bashing my head against wall.
	if (gameData->gameSet.isLoadUi) {
		ui->showInterface(GetFileNameWithoutExt(gameData->gameSet.defaultLoadingPath.c_str()));
	}

	if (gameData->gameSet.defaultLoadingPath.empty()) {
		world->setRoomBin(gameData->rooms.at(0));
	} else {
		std::string chosenName = GetFileNameWithoutExt(gameData->gameSet.defaultLoadingPath.c_str());
		for (auto &room : gameData->rooms) {
			if (room.name == chosenName) {
				world->setRoomBin(room);
				break;
			}
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
}

void Game::setLua(sol::state_view lua) { scripts->setLua(lua); }
