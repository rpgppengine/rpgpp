#ifndef _RPGPP_GAME_H
#define _RPGPP_GAME_H

#include "scriptService.hpp"
#include "sol/state_view.hpp"
class WorldService;

#include <memory>
#include <string>

#include "gamedata.hpp"
#include "interfaceService.hpp"
#include "resourceService.hpp"
#include "soundService.hpp"
#include "stateService.hpp"
#include "worldService.hpp"

#define RPGPP_VER "0.1"

class Game {
private:
	static Game *instance_;
	static std::unique_ptr<GameData> gameData;
	static bool usesBin;
	static bool willClose;
	static std::unique_ptr<StateService> state;
	static std::unique_ptr<WorldService> world;
	static std::unique_ptr<InterfaceService> ui;
	static std::unique_ptr<ResourceService> resources;
	static std::unique_ptr<SoundService> sounds;
	static std::unique_ptr<ScriptService> scripts;

public:
	Game();
	static Game &instance();
	static Game *instancePtr();
	static bool isUsingBin();
	static void useBin(const std::string &filePath);
	static GameData &getBin();
	static StateService &getState();
	static WorldService &getWorld();
	static InterfaceService &getUi();
	static ResourceService &getResources();
	static SoundService &getSounds();
	static ScriptService &getScripts();

	static void init(bool useBin = false);

	static void update();
	static void draw();
	static void unload();

	static void setLua(sol::state_view lua);

	static void closeGame();
	static bool windowWillClose();
};

#endif
