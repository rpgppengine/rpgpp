#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <raylib.h>

#include <array>
#include <cereal/types/array.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "actor.hpp"
#include "basicTypes.hpp"
#include "cereal/cereal.hpp"
#include "dialogueBalloon.hpp"
#include "drawHelper.hpp"
#include "prop.hpp"
#include "resource.hpp"
#include "roomtypes.hpp"
#include "tileset.hpp"

struct ProjectProgramSettings {
	std::string projectTitle;
	std::string projectVersion;
	IVector windowSize = {640, 480};
	std::string programIconPath = "";
	bool windowResizeableFlag = false;
	int targetFPS = 60;
	int windowStateFlag = 0;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(projectTitle), CEREAL_NVP(projectVersion), CEREAL_NVP(windowSize), CEREAL_NVP(programIconPath),
		  CEREAL_NVP(windowResizeableFlag), CEREAL_NVP(targetFPS), CEREAL_NVP(windowStateFlag));
	}
};

struct ProjectGameSettings {
	std::string defaultLoadingPath;
	bool isLoadUi;
	std::string playerActorPath;
	int tileSize = 16;
	bool debugDraw = false;
	std::vector<int> exportImageScales = {1};
	std::vector<int> exportFontSizes = {13};

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(defaultLoadingPath), CEREAL_NVP(isLoadUi), CEREAL_NVP(playerActorPath), CEREAL_NVP(tileSize),
		  CEREAL_NVP(debugDraw), CEREAL_NVP(exportImageScales), CEREAL_NVP(exportFontSizes));
	}
};

struct GameData {
	std::string title;
	ProjectProgramSettings programSet;
	ProjectGameSettings gameSet;

	std::map<std::string, FontBin> fonts;
	std::map<std::string, ImageBin> images;

	// std::map<std::string, TileSetBin> tilesets;
	std::map<std::string, TileSet> tilesets;

	std::map<std::string, InteractableBin> interactables;
	std::vector<RoomBin> rooms;
	std::map<std::string, ActorBin> actors;
	// std::map<std::string, Actor> actors;
	std::vector<PropBin> props;
	// std::vector<Prop> props;
	std::map<std::string, InterfaceViewBin> interfaceViews;
	std::map<std::string, DialogueBin> dialogues;
	std::map<std::string, MusicBin> music;
	std::map<std::string, ScriptBin> scripts;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(programSet), CEREAL_NVP(gameSet), CEREAL_NVP(fonts), CEREAL_NVP(images),
		  CEREAL_NVP(tilesets), CEREAL_NVP(interactables), CEREAL_NVP(rooms), CEREAL_NVP(actors), CEREAL_NVP(props),
		  CEREAL_NVP(interfaceViews), CEREAL_NVP(dialogues), CEREAL_NVP(music), CEREAL_NVP(scripts));
	}
};

struct MinData {
	std::string title;
	ProjectProgramSettings programSet;
	ProjectGameSettings gameSet;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(programSet), CEREAL_NVP(gameSet));
	}
};

struct DataSerialization {
	std::vector<uint8_t> bytes;
	int count;
};

void serializeDataToFile(std::string fileName, GameData data);
GameData deserializeFile(std::string fileName);

#endif
