#include "services/resources.hpp"

#include <stdexcept>

#include "raylib.h"
#include "utils/recentProjects.hpp"

using namespace editor;

Resources::Resources() { loadResources(); }

void Resources::unload() {
	for (auto &[title, font] : fonts) {
		if (IsFontValid(font)) {
			UnloadFont(font);
		}
	}
	for (auto &[title, texture] : textures) {
		if (IsTextureValid(texture)) {
			UnloadTexture(texture);
		}
	}
}

void Resources::loadResources() {
	auto files = LoadDirectoryFilesEx("resources", ".ttf;.otf;.png;.jpg", false);
	for (int i = 0; i < files.count; i++) {
		std::string title = GetFileName(files.paths[i]);
		if (IsFileExtension(files.paths[i], ".ttf") || IsFileExtension(files.paths[i], ".otf")) {
			loadFont(title, files.paths[i]);
		}
		if (IsFileExtension(files.paths[i], ".png") || IsFileExtension(files.paths[i], ".jpg")) {
			loadTexture(title, files.paths[i]);
		}
	}

	UnloadDirectoryFiles(files);
}

void Resources::loadFont(const std::string &title, const std::string &path) {
	auto font = LoadFont(path.c_str());
	fonts.emplace(title, font);
}

void Resources::loadTexture(const std::string &title, const std::string &path) {
	auto texture = LoadTexture(path.c_str());
	textures.emplace(title, texture);
}

Texture Resources::getTexture(const std::string &title) {
	if (textures.count(title) == 0) {
		throw std::runtime_error("Such texture does not exist: " + title);
	}
	return textures.at(title);
}

Font Resources::getFont(const std::string &title) { return fonts.at(title); }

RecentProjects &Resources::getRecentProjects() { return recentProjects; }
