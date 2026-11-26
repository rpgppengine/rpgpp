#include "widgets/filePanels/tileSetPanel.hpp"

#include <memory>
#include <string>
#include <utility>

#include "basicTypes.hpp"
#include "commands/propSet.hpp"
#include "services/interface.hpp"
#include "tileset.hpp"
#include "widgets/propertyList.hpp"
#include "widgets/values/valuewidget.hpp"
#include "widgets/worldView/tileSetCanvas.hpp"

using namespace editor;

TileSetPanel::TileSetPanel() {
	// Properties
	properties = std::make_shared<PropertyList>();
	properties->translationId = "TileSet.Props";
	properties->setPosition({1, -PROPERTIES_WIDTH}, {0, 0});
	properties->setSize({0, PROPERTIES_WIDTH}, {1, 0});
	add(properties);

	bindProp<TileSet, int>("Width", TILESET_WIDTH, &TileSet::getTileWidth, &TileSet::setTileWidth);
	bindProp<TileSet, int>("Height", TILESET_HEIGHT, &TileSet::getTileHeight, &TileSet::setTileHeight);
	bindProp<TileSet, ImageRef>("Texture", TILESET_IMG, &TileSet::getTextureSourceRef, &TileSet::setTextureSourceRef);

	// world view
	canvas = std::make_shared<TileSetCanvas>();
	canvas->setSize({1, -(PROPERTIES_WIDTH + INTERFACE_SPACING)}, {1, 0});
	add(canvas);
}

void TileSetPanel::init() {
	auto &tileset = resource->as<TileSet>();
	properties->getProperty(TILESET_WIDTH)->setValue(tileset.getTileWidth());
	properties->getProperty(TILESET_HEIGHT)->setValue(tileset.getTileHeight());
	properties->getProperty(TILESET_IMG)->setValue(ImageRef{tileset.getTextureSource()});

	initTexture();
}

void TileSetPanel::refresh() { initTexture(); }

void TileSetPanel::initTexture() {
	auto &tileset = resource->as<TileSet>();

	auto texture = tileset.getTexture();

	auto &canvasGrid = canvas->getGrid();
	canvasGrid.tileWidth = tileset.getTileWidth();
	canvasGrid.tileHeight = tileset.getTileHeight();
	canvasGrid.rows = (texture.width / canvasGrid.tileWidth);
	canvasGrid.columns = (texture.height / canvasGrid.tileHeight);

	canvas->setTexture(texture);
}
