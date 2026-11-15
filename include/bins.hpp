#ifndef _RPGPP_BINS_H
#define _RPGPP_BINS_H

#include <string>
#include <vector>

#include "basicTypes.hpp"
#include "cereal/cereal.hpp"
#include "roomtypes.hpp"

struct ActorBin {
	std::string name;
	std::string tileSetName;
	IRect collision;
	std::array<std::vector<IVector>, 8> animations;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(tileSetName), CEREAL_NVP(collision), CEREAL_NVP(animations));
	}
};

struct InteractableBin {
	std::string typeName;
	std::string scriptPath;
	std::vector<std::uint8_t> props;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(typeName), CEREAL_NVP(scriptPath), CEREAL_NVP(props));
	}
};

struct ScriptBin {
	std::string bytecode;

	template <typename Archive>
	void serialize(Archive &a) {
		a(bytecode);
	}
};

struct TileSetBin {
	std::string name;
	std::string extension;
	IVector tileSize;
	std::vector<unsigned char> image;
	int dataSize;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(extension), CEREAL_NVP(tileSize), CEREAL_NVP(image), CEREAL_NVP(dataSize));
	}
};

struct FontBin {
	std::vector<unsigned char> data;
	int dataSize;
	std::string ext;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(data), CEREAL_NVP(dataSize), CEREAL_NVP(ext));
	}
};

struct ImageBin {
	std::vector<unsigned char> data;
	int dataSize;
	std::string ext;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(data), CEREAL_NVP(dataSize), CEREAL_NVP(ext));
	}
};

struct MusicBin {
	std::string relativePath;
	bool isSound;
	std::vector<unsigned char> fileData;
	std::string fileExt;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(relativePath), CEREAL_NVP(isSound), CEREAL_NVP(fileData), CEREAL_NVP(fileExt));
	}
};

struct PropBin {
	std::string name;
	IRect atlasRect;
	IRect collisionRect;
	std::string imagePath;
	bool hasInteractable;
	std::string intType;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(atlasRect), CEREAL_NVP(collisionRect), CEREAL_NVP(imagePath),
		  CEREAL_NVP(hasInteractable), CEREAL_NVP(intType));
	}
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

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(tileSetName), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(startPoint),
		  CEREAL_NVP(collisions), CEREAL_NVP(interactables), CEREAL_NVP(props), CEREAL_NVP(actors), CEREAL_NVP(tiles),
		  CEREAL_NVP(musicSource));
	}
};

struct UIElementBin {
	std::string type = "";
	std::unordered_map<std::string, ElementProperty> props;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(type), CEREAL_NVP(props));
	}
};

struct InterfaceViewBin {
	std::string scriptSource = "";
	std::unordered_map<std::string, std::vector<std::uint8_t>> entites;
	std::unordered_map<std::string, UIElementBin> elements = {};

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(scriptSource), CEREAL_NVP(entites), CEREAL_NVP(elements));
	}
};

#endif
