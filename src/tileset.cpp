#include "tileset.hpp"

#include <raylib.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "atlasTile.hpp"
using json = nlohmann::json;

TileSet::TileSet() : texture(), tileSize(Vector2{16, 16}) {}

TileSet::TileSet(const std::string &textureSource, Vector2 tileSize) {
	this->textureSource = textureSource;
	this->texture = LoadTexture(textureSource.c_str());
	this->tileSize = tileSize;
}

TileSet::TileSet(const std::string &textureSource, int tileSizeInt)
	: TileSet(textureSource, Vector2{static_cast<float>(tileSizeInt), static_cast<float>(tileSizeInt)}) {}

TileSet::TileSet(const Texture &texture, Vector2 tileSize) {
	this->texture = texture;
	this->tileSize = tileSize;
}

TileSet::TileSet(const std::string &fileName) { loadFile(fileName); }

TileSet::TileSet(TileSetBin bin) {
	this->tileSize = Vector2{static_cast<float>(bin.tileSize.x), static_cast<float>(bin.tileSize.y)};

	unsigned char *imageData = bin.image.data();
	Image image = LoadImageFromMemory(bin.extension.c_str(), imageData, bin.dataSize);

	this->texture = LoadTextureFromImage(image);
}

TileSet::~TileSet() {
	// UnloadTexture(texture);
}

/*
json TileSet::dumpJson() {
	json tileSetJson = {
		{"source", textureSource}, {"tileSize", tileSize.x}, {"tileWidth", tileSize.x}, {"tileHeight", tileSize.y}};

	return tileSetJson;
}
*/

void TileSet::unload() const { UnloadTexture(texture); }

Vector2 TileSet::getTileSize() const { return this->tileSize.toFloat(); }

void TileSet::setTileSize(int size) { this->tileSize = Vector2{static_cast<float>(size), static_cast<float>(size)}; }

int TileSet::getTileWidth() { return static_cast<int>(tileSize.x); }

int TileSet::getTileHeight() { return static_cast<int>(tileSize.y); }

void TileSet::setTileWidth(int value) { tileSize.x = static_cast<float>(value); }

void TileSet::setTileHeight(int value) { tileSize.y = static_cast<float>(value); }

void TileSet::setTileSizeVector(Vector2 vec) { this->tileSize = vec; }

Texture TileSet::getTexture() const { return this->texture; }

std::string TileSet::getTextureSource() { return this->textureSource; }

std::string TileSet::getSource() { return this->source; }

void TileSet::setTextureSource(const std::string &source) {
	this->textureSource = source;
	this->texture = LoadTexture(source.c_str());
}

bool TileSet::areAtlasCoordsValid(Vector2 atlasCoords) const {
	bool xValid = false;
	bool yValid = false;
	bool posValid = false;

	if (atlasCoords.x >= 0 && atlasCoords.x < (texture.width / static_cast<float>(tileSize.x))) {
		xValid = true;
	}
	if (atlasCoords.y >= 0 && atlasCoords.y < (texture.height / static_cast<float>(tileSize.y))) {
		yValid = true;
	}

	posValid = (xValid && yValid);
	return posValid;
}

AtlasTile TileSet::getTile(Vector2 atlasCoords) {
	AtlasTile tile(&this->texture, atlasCoords);

	return tile;
}
