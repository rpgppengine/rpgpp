#include "utils/resourceTypeManager.hpp"

#include <algorithm>
#include <vector>

#include "actor.hpp"
#include "dialogue.hpp"
#include "edui/fileChooser.hpp"
#include "icons.hpp"
#include "interfaceView.hpp"
#include "prop.hpp"
#include "room.hpp"
#include "tileset.hpp"

using namespace editor;

ResourceTypeManager::ResourceTypeManager() { addDefaults(); }

void ResourceTypeManager::addDefaults() {
	// common file types
	addCategory("Images", {".png", ".jpg"}, ICON_FILETYPE_IMAGE);
	addCategory("Text file", {".txt"}, ICON_FILETYPE_TEXT);
	addCategory("Audio", {".wav", ".mp4", ".ogg"}, ICON_FILETYPE_AUDIO);
	addCategory("Font", {".ttf"}, ICON_FILETYPE_FONT);
	// RPG++ project file
	addCategory("RPG++ Project", {".rpgpp"}, ICON_PRIORITY);
	// RPG++ defaults
	addCategory("TileSet", {".rtiles"}, ICON_BOX_GRID_BIG);
	addCategory("Room", {".rmap"}, ICON_FILETYPE_HOME);
	addCategory("Actor", {".ractor"}, ICON_PLAYER);
	addCategory("Prop", {".rprop"}, ICON_CUBE);
	addCategory("Dialogue", {".rdiag"}, ICON_TEXT_POPUP);
	addCategory("UI View", {".rui"}, ICON_WINDOW);
	addCategory("Script", {".lua"}, ICON_FILETYPE_CODE_XML);

	// factories
	addT<TileSet>("TileSet");
	addT<Room>("Room");
	addT<Actor>("Actor");
	addT<Prop>("Prop");
	addT<Dialogue>("Dialogue");
	addT<InterfaceView>("UI View");
}

const std::vector<edui::FileCategory> &ResourceTypeManager::getCategories() { return types; }

const edui::FileCategory &ResourceTypeManager::getCategory(const std::string &title) {
	for (auto &item : types) {
		if (item.title == title) {
			return item;
		}
	}
	return types.front();
}

int ResourceTypeManager::getIconByExtension(const std::string &ext) {
	for (auto &item : types) {
		if (std::find(item.extensions.begin(), item.extensions.end(), ext) != item.extensions.end()) {
			return item.iconId;
		}
	}
	return ICON_FILE;
}

const edui::FileCategory &ResourceTypeManager::getCategoryBYExtension(const std::string &ext) {
	for (auto &item : types) {
		if (std::find(item.extensions.begin(), item.extensions.end(), ext) != item.extensions.end()) {
			return item;
		}
	}
	return types.front();
}

void ResourceTypeManager::addCategory(const std::string &title, std::vector<std::string> extensions, int iconId) {
	edui::FileCategory category;
	category.title = title;
	for (int i = 0; i < (extensions.size() < edui::FILE_CATEGORY_MAX_EXTENSIONS ? extensions.size()
																				: edui::FILE_CATEGORY_MAX_EXTENSIONS);
		 i++) {
		category.extensions[i] = extensions[i];
	}
	category.iconId = iconId;

	types.push_back(category);
}

bool ResourceTypeManager::hasFactory(const std::string &title) { return factories.count(title) > 0; }

ResourceFactory &ResourceTypeManager::getFactory(const std::string &title) { return factories[title]; }
