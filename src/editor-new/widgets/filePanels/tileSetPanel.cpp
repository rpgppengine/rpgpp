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

	auto width = properties->addPropertyDefault<int>("Width", TILESET_WIDTH);
	width->as<ValueWidgetT<int>>().onValueChanged.connect([this](int a, int b) {
		std::unique_ptr<PropSet<int>> cmd = std::make_unique<PropSet<int>>();
		auto &tileset = resource->as<TileSet>();
		cmd->newValue = b;
		cmd->getter = [&tileset] { return tileset.getTileWidth(); };
		cmd->setter = [&tileset](int a) { tileset.setTileWidth(a); };

		this->exec(std::move(cmd));
		// init();
	});

	auto height = properties->addPropertyDefault<int>("Height", TILESET_HEIGHT);
	height->as<ValueWidgetT<int>>().onValueChanged.connect([this](int a, int b) {
		auto cmd = std::make_unique<PropSet<int>>();
		auto &tileset = resource->as<TileSet>();
		cmd->newValue = b;
		cmd->getter = [&tileset] { return tileset.getTileHeight(); };
		cmd->setter = [&tileset](int a) { tileset.setTileHeight(a); };

		this->exec(std::move(cmd));
		// init();
	});

	auto textureSource = properties->addPropertyDefault<ImageRef>("Texture", TILESET_IMG);
	textureSource->as<ValueWidgetT<ImageRef>>().onValueChanged.connect([this](ImageRef a, ImageRef b) {
		auto cmd = std::make_unique<PropSet<std::string>>();
		auto &tileset = resource->as<TileSet>();
		cmd->newValue = b.path;
		cmd->getter = [&tileset] { return tileset.getTextureSource(); };
		cmd->setter = [&tileset](std::string a) { tileset.setTextureSource(a); };

		this->exec(std::move(cmd));
		initTexture();
		// init();
	});

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
