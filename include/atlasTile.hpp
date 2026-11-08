#ifndef _RPGPP_ATLASTILE_H
#define _RPGPP_ATLASTILE_H

#include <raylib.h>

#include "basicTypes.hpp"

/**
 * Represents a source atlas tile from a TileSet.
 */
class AtlasTile {
private:
	/** Pointer to the used Texture */
	Texture *texture;
	/** The atlas (source) coordinates from the TileSet. */
	IVector atlasCoords;

public:
	/**
	 * Empty constructor
	 */
	AtlasTile();

	/**
	 * Constructor that takes in a texture and coordinates on the atlas (the
	 * tileset)
	 */
	AtlasTile(Texture *texture, IVector atlasCoords);

	/**
	 * Get the atlas coordinates
	 * @returns An IVector which represents the atlas coordinates
	 */
	IVector getAtlasCoords() const;
};

#endif
