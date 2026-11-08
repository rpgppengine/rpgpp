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
#include <string>
#include <unordered_map>
#include <vector>

#include "basicTypes.hpp"
#include "dialogueBalloon.hpp"
#include "roomtypes.hpp"

#define RPGPP_DRAW_MULTIPLIER (3)

struct ActorBin {
	std::string name;
	std::string tileSetName;
	IRect collision;
	std::array<std::vector<IVector>, 8> animations;
};

struct InteractableBin {
	std::string typeName;
	std::string scriptPath;
	std::vector<std::uint8_t> props;
};

struct ScriptBin {
	std::string bytecode;
};

struct TileSetBin {
	std::string name;
	std::string extension;
	IVector tileSize;
	std::vector<unsigned char> image;
	int dataSize;
};

struct FontBin {
	std::vector<unsigned char> data;
	int dataSize;
	std::string ext;
};

struct ImageBin {
	std::vector<unsigned char> data;
	int dataSize;
	std::string ext;
};

struct MusicBin {
	std::string relativePath;
	bool isSound;
	std::vector<unsigned char> fileData;
	std::string fileExt;
};

struct PropBin {
	std::string name;
	IRect atlasRect;
	IRect collisionRect;
	std::string imagePath;
	bool hasInteractable;
	std::string intType;
};

struct RoomBin {
	std::string name;
	std::string tileSetName;
	int width;
	int height;
	IVector startPoint;
	std::vector<IVector> collisions;
	std::vector<InteractableInRoomBin> interactables;
	std::vector<PropInRoomBin> props;
	std::vector<ActorInRoomBin> actors;
	std::vector<std::vector<TileBin>> tiles;
	std::string musicSource;
};

struct UIElementBin {
	std::string type = "";
	std::unordered_map<std::string, ElementProperty> props;
};

struct InterfaceViewBin {
	std::string scriptSource = "";
	std::unordered_map<std::string, std::vector<std::uint8_t>> entites;
	std::unordered_map<std::string, UIElementBin> elements = {};
};

struct ProjectProgramSettings {
	std::string projectTitle;
	std::string projectVersion;
	IVector windowSize = {640, 480};
	std::string programIconPath = "";
	bool windowResizeableFlag = false;
	int targetFPS = 60;
	int windowStateFlag = 0;
};

struct ProjectGameSettings {
	std::string defaultLoadingPath;
	bool isLoadUi;
	std::string playerActorPath;
	int tileSize = 16;
	bool debugDraw = false;
	std::vector<int> exportImageScales = {1};
	std::vector<int> exportFontSizes = {13};
};

struct GameData {
	std::string title;
	ProjectProgramSettings programSet;
	ProjectGameSettings gameSet;
	std::map<std::string, FontBin> fonts;
	std::map<std::string, ImageBin> images;
	std::map<std::string, TileSetBin> tilesets;
	std::map<std::string, InteractableBin> interactables;
	std::vector<RoomBin> rooms;
	std::map<std::string, ActorBin> actors;
	std::vector<PropBin> props;
	std::map<std::string, InterfaceViewBin> interfaceViews;
	std::map<std::string, DialogueBin> dialogues;
	std::map<std::string, MusicBin> music;
	std::map<std::string, ScriptBin> scripts;
};

struct DataSerialization {
	std::vector<uint8_t> bytes;
	int count;
};

void serializeDataToFile(std::string fileName, GameData data);
GameData deserializeFile(std::string fileName);

template <class Archive>
void serialize(Archive &a, UIElementBin &b) {
	a(b.type, b.props);
}

template <class Archive>
void serialize(Archive &a, InterfaceViewBin &b) {
	a(b.scriptSource, b.entites, b.elements);
}

#endif
