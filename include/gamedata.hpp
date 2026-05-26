#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <raylib.h>

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "dialogueBalloon.hpp"

#define RPGPP_DRAW_MULTIPLIER (3)

struct IVector {
	int x;
	int y;

	bool operator==(const IVector &other) const { return x == other.x && y == other.y; }

	bool operator<(const IVector &other) const { return x < other.x || (x == other.x && y < other.y); }

	IVector() = default;
	IVector(int x, int y) {
		this->x = x;
		this->y = y;
	}
	IVector(const Vector2 &from) {
		x = static_cast<int>(from.x);
		y = static_cast<int>(from.y);
	}
};

struct IRect {
	int x;
	int y;
	int width;
	int height;
};

struct UIElementRef {
	std::string title = "";
	uint16_t entityId = 256;
};

struct FontRef {
	std::string path = "";
	Font font;
};

struct ImageRef {
	std::string path = "";
	Texture texture;
};

enum TextAlignment {
	TEXT_ALIGN_LEFT = 0,
	TEXT_ALIGN_TOP = 0,
	TEXT_ALIGN_CENTRE = 1,
	TEXT_ALIGN_MIDDLE = 1,
	TEXT_ALIGN_RIGHT = 2,
	TEXT_ALIGN_BOTTOM = 2
};

enum InterfaceElementType {
	INTERFACE_NULL,
	INTERFACE_TEXTAREA,
	INTERFACE_LABEL,
	INTERFACE_COLORRECT,
	INTERFACE_IMAGERECT,
	INTERFACE_NINEPATCHIMAGERECT,
	INTERFACE_BUTTON,
	INTERFACE_DIALOGUEAREA
};

#define RPGPP_INTERFACE_ELEMENT_MAX (8)

typedef std::variant<int, std::string, Rectangle, Color, NPatchInfo, UIElementRef, TextAlignment> PropVariant;

struct UIElementBin {
	InterfaceElementType type;
	int layer;
	std::map<std::string, PropVariant> props;
};

struct ActorBin {
	std::string name;
	std::string tileSetName;
	IRect collision;
	std::array<std::vector<IVector>, 8> animations;
};

struct ActorInRoomBin {
	std::string name;
	std::string source;
	IVector tilePos;
	std::string intType;
	std::vector<std::uint8_t> propsCbor;
};

struct TileBin {
	IVector atlasPos;
	IVector worldPos;
};

struct InteractableBin {
	std::string typeName;
	std::string scriptPath;
	std::vector<std::uint8_t> props;
};

struct ScriptBin {
	std::string bytecode;
};

struct InteractableInRoomBin {
	int x;
	int y;
	bool onTouch;
	std::string type;
	std::vector<std::uint8_t> propsCbor;
};

struct PropInRoomBin {
	std::string name;
	IVector tilePos;
	bool onTouch;
	std::vector<std::uint8_t> propsCbor;
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

struct InterfaceViewBin {
	std::map<std::string, UIElementBin> elements;
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
	std::string defaultRoomPath;
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

#endif
