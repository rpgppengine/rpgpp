#include "atlasTile.hpp"

#include <raylib.h>

#include "basicTypes.hpp"

AtlasTile::AtlasTile() {
	this->texture = nullptr;
	this->atlasCoords = Vector2{-1, -1};
}

AtlasTile::AtlasTile(Texture *texture, IVector atlasCoords) {
	this->texture = texture;
	this->atlasCoords = atlasCoords;
}

IVector AtlasTile::getAtlasCoords() const { return this->atlasCoords; }
