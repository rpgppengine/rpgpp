#include "atlasTile.hpp"

#include <raylib.h>

#include "basicTypes.hpp"

AtlasTile::AtlasTile() { this->atlasCoords = Vector2{-1, -1}; }

AtlasTile::AtlasTile(Texture texture, IVector atlasCoords) {
	this->texture = texture;
	this->atlasCoords = atlasCoords;
}

IVector AtlasTile::getAtlasCoords() const { return this->atlasCoords; }

void AtlasTile::setAtlasCoords(IVector atlasCoords) { this->atlasCoords = atlasCoords; }

Texture AtlasTile::getTexture() const { return texture; }

void AtlasTile::setTexture(Texture texture) { this->texture = texture; }
