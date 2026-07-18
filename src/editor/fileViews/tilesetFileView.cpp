#include "fileViews/tilesetFileView.hpp"

#include <algorithm>

#include "TGUI/Widgets/Group.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/translationService.hpp"
#include "tileset.hpp"
#include "variant.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
TileSetFileView::TileSetFileView() {
	TranslationService &ts = Editor::instance->getTranslations();

	const auto worldView = TileSetView::create();
	worldView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	widgetContainer.push_back(worldView);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(worldView));

	this->worldView = worldView;

	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	widthField = IntField::create();
	bindTranslation(widthField->label, "screen.project.tilesetview.tile_width", &tgui::Label::setText);
	widthField->value->onValueChange(
		[this](const float &value) {
			this->dirty = true;
			this->worldView->getTileSet()->setTileWidth(static_cast<int>(value));
		});
	props->addIntField(widthField);

	heightField = IntField::create();
	bindTranslation(heightField->label, "screen.project.tilesetview.tile_height", &tgui::Label::setText);
	heightField->value->onValueChange(
		[this](const float &value) {
			this->dirty = true;
			this->worldView->getTileSet()->setTileHeight(static_cast<int>(value));
		});

	props->addButton("Square Tiles", [this] {
		auto tileset = this->worldView->getTileSet();
		Vector2 tileSize = tileset->getTileSize();
		float min = std::min(tileSize.x, tileSize.y);
		tileset->setTileSizeVector({min, min});

		widthField->value->setValue(min);
		heightField->value->setValue(min);
		this->dirty = true;
	});

	props->addIntField(widthField);
	props->addIntField(heightField);

	textureFile = FileField::create("", "...");
	bindTranslation(textureFile->label, "screen.project.tilesetview.texture", &tgui::Label::setText);
	textureFile->pathFilters = {{
		{"Images", {"*.png", "*.jpg"}},
	}};
	textureFile->callback = [this](const tgui::String &path) {
		auto tileset = this->worldView->getTileSet();
		tileset->setTextureSource(path.toStdString());
		this->dirty = true;
	};
	props->addFileField(textureFile);

	widgetContainer.push_back(props);
}

void TileSetFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<TileSet> *>(variant);
		const auto tileset = ptr->get();

		worldView->setTileSet(tileset);

		widthField->value->setValue(tileset->getTileWidth());
		heightField->value->setValue(tileset->getTileHeight());
		textureFile->setValue(GetFileName(tileset->getTextureSource().c_str()));

		addWidgets(layout);
		this->dirty = false;
	}
}
