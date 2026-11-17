#include "utils/resourceTypeManager.hpp"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <vector>

#include "actor.hpp"
#include "dialogue.hpp"
#include "edui/fileChooser.hpp"
#include "icons.hpp"
#include "interfaceView.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "resource.hpp"
#include "room.hpp"
#include "tileset.hpp"

using namespace editor;

ResourceTypeManager::ResourceTypeManager() { addDefaults(); }

void ResourceTypeManager::addDefaults() {
	// common file types
	addCategory("Image", {".png", ".jpg"}, ICON_FILETYPE_IMAGE);
	addCategory("Text", {".txt"}, ICON_FILETYPE_TEXT);
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
	addT<TileSet>("TileSet", RPGPP_RESOURCE_TILESET);
	addT<Room>("Room", RPGPP_RESOURCE_ROOM);
	addT<Actor>("Actor", RPGPP_RESOURCE_ACTOR);
	addT<Prop>("Prop", RPGPP_RESOURCE_PROP);
	addT<Dialogue>("Dialogue", RPGPP_RESOURCE_DIALOGUE);
	addT<InterfaceView>("UI View", RPGPP_RESOURCE_UI);
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

bool ResourceTypeManager::getTypeTitleByExtension(const std::string &ext, std::string *title) {
	bool result = false;

	for (auto &item : types) {
		if (std::find(item.extensions.begin(), item.extensions.end(), ext) != item.extensions.end()) {
			*title = item.title;
			result = true;
		}
	}

	return result;
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

RPGPP_RESOURCE ResourceTypeManager::getEnum(const std::string &title) {
	auto factory = getFactory(title);
	return factory.typeId;
}

std::vector<std::filesystem::path> ResourceTypeManager::findAllOfType(const std::filesystem::path &base,
																	  const std::string &title) {
	auto cat = getCategory(title);
	std::string filter = "";
	for (auto &item : cat.extensions) {
		filter = filter.append(item.c_str());
		filter = filter.append(";");
	}

	std::vector<std::filesystem::path> res = {};
	auto list = LoadDirectoryFilesEx(base.c_str(), filter.c_str(), true);
	for (int i = 0; i < list.count; i++) {
		res.push_back(list.paths[i]);
	}
	UnloadDirectoryFiles(list);

	return res;
}

size_t ResourceTypeManager::totalResourceCount(const std::filesystem::path &base) {
	size_t result = 0;
	for (auto &cat : types) {
		auto vec = findAllOfType(base, cat.title);
		size_t count = vec.size();
		result += count;
	}
	return result;
}
