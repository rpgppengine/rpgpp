#include "gamedata.hpp"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

#include "dialogueBalloon.hpp"

template <class Archive>
void serialize(Archive &a, ActorBin &b) {
	a(b.name, b.tileSetName, b.collision, b.animations);
}

template <class Archive>
void serialize(Archive &a, InteractableBin &b) {
	a(b.typeName, b.scriptPath, b.props);
}

template <class Archive>
void serialize(Archive &a, TileSetBin &b) {
	a(b.name, b.extension, b.tileSize, b.image, b.dataSize);
}

template <class Archive>
void serialize(Archive &a, FontBin &b) {
	a(b.data, b.dataSize, b.ext);
}

template <class Archive>
void serialize(Archive &a, ImageBin &b) {
	a(b.data, b.dataSize, b.ext);
}

template <class Archive>
void serialize(Archive &a, MusicBin &b) {
	a(b.relativePath, b.isSound, b.fileData, b.fileExt);
}

template <class Archive>
void serialize(Archive &a, PropBin &b) {
	a(b.name, b.atlasRect, b.collisionRect, b.imagePath, b.hasInteractable, b.intType);
}

template <class Archive>
void serialize(Archive &a, RoomBin &b) {
	a(b.name, b.tileSetName, b.width, b.height, b.startPoint, b.collisions, b.interactables, b.props, b.actors, b.tiles,
	  b.musicSource);
}

template <class Archive>
void serialize(Archive &a, ScriptBin &b) {
	a(b.bytecode);
}

/*
	template <class Archive>
void serialize(Archive &a, InterfaceViewBin &b) {
		a(b.scriptSource, b.entites, b.elements);
}
 */

template <class Archive>
void serialize(Archive &a, ProjectProgramSettings &b) {
	a(b.projectTitle, b.projectVersion, b.windowSize, b.programIconPath, b.windowResizeableFlag, b.windowStateFlag,
	  b.targetFPS);
}

template <class Archive>
void serialize(Archive &a, ProjectGameSettings &b) {
	a(b.defaultLoadingPath, b.isLoadUi, b.playerActorPath, b.tileSize, b.debugDraw, b.exportImageScales,
	  b.exportFontSizes);
}

template <class Archive>
void serialize(Archive &a, GameData &b) {
	a(b.title, b.programSet, b.gameSet, b.images, b.fonts, b.tilesets, b.rooms, b.actors, b.props, b.dialogues, b.music,
	  b.interactables, b.scripts, b.interfaceViews);
}

void serializeDataToFile(std::string fileName, GameData data) {
	{
		std::fstream os{fileName, std::ifstream::binary | std::fstream::trunc | std::fstream::out};
		if (!os.is_open()) {
			printf("Cannot open %s for writing. \n", fileName.c_str());
			return;
		}

		cereal::PortableBinaryOutputArchive outa(os);
		outa(data);
	}
	printf("tried to serialize.. \n");
}

GameData deserializeFile(std::string fileName) {
	GameData data{};
	{
		std::ifstream is;
		is.open(fileName, std::ios::in | std::ios::binary);
		cereal::PortableBinaryInputArchive ina(is);
		ina(data);
	}

	return data;
}
