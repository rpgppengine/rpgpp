#ifndef ROOMTYPES_H
#define ROOMTYPES_H

#include "basicTypes.hpp"
struct TileBin {
	IVector worldPos;
	IVector atlasPos;

	template <typename Archive>
	void serialize(Archive &a) {
		a(atlasPos);
	}
};

struct ActorInRoomBin {
	std::string name;
	std::string source;
	IVector tilePos;
	std::string intType;
	std::vector<std::uint8_t> propsCbor;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(source), CEREAL_NVP(intType), CEREAL_NVP(propsCbor));
	}
};

struct PropInRoomBin {
	std::string name;
	IVector tilePos;
	bool onTouch;
	std::vector<std::uint8_t> propsCbor;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(name), CEREAL_NVP(onTouch), CEREAL_NVP(propsCbor));
	}
};

struct InteractableInRoomBin {
	bool onTouch;
	std::string type;
	std::vector<std::uint8_t> propsCbor;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(type), CEREAL_NVP(onTouch), CEREAL_NVP(propsCbor));
	}
};

struct RoomData {
	std::vector<TileBin> tiles;
	std::vector<ActorInRoomBin> actors;
	std::vector<InteractableInRoomBin> interactables;
	std::vector<PropInRoomBin> props;
	std::vector<IVector> collisions;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(tiles), CEREAL_NVP(actors), CEREAL_NVP(interactables), CEREAL_NVP(props));
	}
};

#endif
