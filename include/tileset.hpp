#ifndef _RPGPP_TILESET_H
#define _RPGPP_TILESET_H

#include <raylib.h>

#include <nlohmann/json.hpp>
#include <string>

#include "basicTypes.hpp"
#include "gamedata.hpp"
#include "resource.hpp"
#include "saveable.hpp"
using json = nlohmann::json;
#include "atlasTile.hpp"

/** The TileSet class represents a collection of tiles that can be used for a
 * TileMap or another object
 * @see [TileMap](TileMap.md)
 */
class TileSet : public Resource {
private:
	/** The used Texture for this TileSet. A raylib Texture is used. */
	Texture texture;
	/** The size of a single file. Tiles can have different widths and heights,
	 * thus a Vector2 is used. */
	IVector tileSize;
	/** The path to the used texture. May be a .png file or other image format.
	 */
	std::string textureSource;
	/** The path to the file that represents this TileSet. */
	std::string source;

public:
	/** Empty constructor. */
	TileSet();
	/** Constructor that takes the path to the .rtiles file */
	TileSet(const std::string &fileName);
	/** Constructor that takes the path of the texture (image file) and a
	 * Vector2 that represents the tile size */
	TileSet(const std::string &textureSource, Vector2 tileSize);
	/** Like TileSet(std::string textureSource, Vector2 tileSize), but takes an
	 * int for tile size with identical width and height (square tiles) */
	TileSet(const std::string &textureSource, int tileSizeInt);
	TileSet(const Texture &texture, Vector2 tileSize);
	/** Constructor that takes the TileSetBin binary structure */
	TileSet(TileSetBin bin);
	~TileSet();
	/** Dump the TileSet's data into a nlohmann::json object */
	// json dumpJson() override;
	/** Unload routine */
	void unload() const;
	/** Get the size of a tile. Since a tile may have different width and
	 * height, it returns a Vector2 */
	Vector2 getTileSize() const;
	/** Set the tile size to an integer value. Both width and height will be
	 * this value. */
	void setTileSize(int size);
	/** Set the tile size to a Vector2 value. The x component is taken for
	 * width; the y component is taken for height. */
	void setTileSizeVector(Vector2 vec);
	/** Get the height of a tile unit in this TileSet. */
	int getTileHeight();
	/** Get the width of a tile unit in this TileSet. */
	int getTileWidth();
	/** Set a tile unit height. */
	void setTileHeight(int value);
	/** Set a tile unit width. */
	void setTileWidth(int value);
	/** Get the texture of this TileSet.
	 * @returns A raylib Texture.
	 */
	Texture getTexture() const;
	/** Get the path to the texture image file. */
	std::string getTextureSource();
	/** Set the TileSet's texture source image. */
	void setTextureSource(const std::string &source);
	/** Get the path to the file that represents this TileSet. */
	std::string getSource();
	/** Check whether the given coordinates are valid */
	bool areAtlasCoordsValid(Vector2 atlasCoords) const;
	/** Get a tile from this TileSet as an AtlasTile. */
	AtlasTile getTile(Vector2 atlasCoords);

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(textureSource), CEREAL_NVP(tileSize));
	}

	void loadFile(std::filesystem::path filePath) override {
		// Get text from JSON file
		this->source = filePath;
		char *jsonContent = LoadFileText(filePath.c_str());
		json j = json::parse(jsonContent);

		// Load Texture
		std::string sourcePath = j.at("source");
		this->textureSource = sourcePath;
		this->texture = LoadTexture(sourcePath.c_str());

		int tileWidth = j.at("tileWidth");
		int tileHeight = j.at("tileHeight");
		this->tileSize = Vector2{static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

		// Unload file's text
		UnloadFileText(jsonContent);
	}

	void saveFile(std::filesystem::path filePath) override {}
};

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(TileSet)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, TileSet)

#endif
