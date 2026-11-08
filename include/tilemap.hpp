#ifndef _RPGPP_TILEMAP_H
#define _RPGPP_TILEMAP_H

#include <raylib.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "atlasTile.hpp"
#include "cereal/cereal.hpp"
#include "gamedata.hpp"
#include "tile.hpp"
#include "tileset.hpp"

#define _RPGPP_TILESIZE (16)

using json = nlohmann::json;

/** A TileMap represents a map of the placed tiles in a Room. */
class TileMap {
private:
	/** Base position of this TileMap. */
	Vector2 basePos = {0, 0};
	/** File path to the source of the used TileSet. */
	std::string tileSetSource = "";
	/** A unique_ptr to the used TileSet. */
	std::unique_ptr<TileSet> tileSet = nullptr;
	int atlasTileSize = 16;
	int worldTileSize = 16 * RPGPP_DRAW_MULTIPLIER;
	/** Width of the TileMap in tiles */
	int width = 8;
	/** Height of the TileMap in height */
	int height = 8;
	int maxAtlasWidth;
	int maxAtlasHeight;
	/** A collection representing the tiles in this map */
	std::vector<std::vector<Tile>> tiles;

public:
	TileMap();
	/** Construct from a file name/path */
	TileMap(const std::string &fileName);
	/** Construct using a TileSet path, width, height */
	TileMap(const std::string &tileSetSource, int width, int height, int atlasTileSize, int worldTileSize);
	/** Constructor using a TileSet object */
	TileMap(const TileSet &tileSet, int width, int height, int atlasTileSize, int worldTileSize);
	/** Constructor using a unique_ptr of a TileSet */
	TileMap(std::unique_ptr<TileSet> tileSetPtr, int width, int height, int atlasTileSize, int worldTileSize);
	/** Constructor using a RoomBin binary structure */
	TileMap(const RoomBin &bin);
	/** Dump the TileMap's data into a nlohmann::json object */
	json dumpJson();
	/** Unload routine. Typically calls raylib's UnloadTexture. */
	void unload() const;
	/** Update routine. */
	void update() const;
	/** Draw routine. */
	void draw() const;
	/** Get the path of the TileSet resource */
	std::string getTileSetSource() const;
	/** Get a pointer to the used TileSet */
	TileSet *getTileSet() const;
	/** Set a new TileSet for this TileMap using a path for the new TileSet */
	void setTileSet(const std::string &tileSetSource);
	int getAtlasTileSize() const;
	int getWorldTileSize() const;
	bool atlasPosIsValid(Vector2 atlasPos) const;
	bool worldPosIsValid(Vector2 atlasPos) const;
	/** Get the Tile at the specified x and y position */
	Tile getTile(int x, int y) const;
	/** Set the tile at the specified world position to be a tile at the
	 * specified atlas position from the TileSet. */
	void setTile(Vector2 worldPos, Vector2 atlasPos);
	/** Set the tile at the specified world position to be empty (not drawn). */
	void setEmptyTile(Vector2 worldPos);
	/** Draw the tile at the specified world x and y coordinates. */
	void drawTile(int x, int y) const;
	void drawTile(Vector2 worldPos, Vector2 atlasPos) const;
	void drawTile(Vector2 worldPos, AtlasTile tile) const;
	Vector2 getMaxAtlasSize() const;
	/** Get the size of this TileMap
	 * @returns the size in a Vector2. The x component is the width; the y
	 * component is the height.
	 */
	Vector2 getMaxWorldSize() const;
	/** Set the size of this TileSet using the provided Vector2 argument. The x
	 * ocmponent will be the new width; the y component will be the new height.
	 */
	void setWorldSize(Vector2 size);

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(tileSetSource), CEREAL_NVP(worldTileSize), CEREAL_NVP(width), CEREAL_NVP(height));
	}
};

#endif
