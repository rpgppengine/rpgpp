#include "resourceService.hpp"

#include <raylib.h>

#include <cstdio>

#include "game.hpp"

ResourceService::ResourceService() { init(); }

void ResourceService::init() {
	unload();

	if (Game::isUsingBin()) {
		for (auto &[id, fontBin] : Game::getBin().fonts) {
			Font font = LoadFontFromMemory(fontBin.ext.c_str(), fontBin.data.data(), fontBin.dataSize, 13, nullptr, 0);
			addFont(id, font);

			// font sizes, defined in the project settings
			for (int fontSize : Game::getBin().gameSet.exportFontSizes) {
				Font font = LoadFontFromMemory(fontBin.ext.c_str(), fontBin.data.data(), fontBin.dataSize, fontSize,
											   nullptr, 0);
				addFont(TextFormat("%s-%i", id.c_str(), fontSize), font);
			}
		}

		for (const auto &[name, data] : Game::getBin().images) {
			// scale 1 - the image as it is
			Image image = LoadImageFromMemory(data.ext.c_str(), data.data.data(), data.dataSize);
			Texture2D texture = LoadTextureFromImage(image);
			addTexture(name, texture);
			UnloadImage(image);

			// scales, defined in the settings of the project
			for (int imageScale : Game::getBin().gameSet.exportImageScales) {
				if (imageScale > 1) {
					Image image = LoadImageFromMemory(data.ext.c_str(), data.data.data(), data.dataSize);
					ImageResizeNN(&image, image.width * imageScale, image.height * imageScale);
					Texture2D texture = LoadTextureFromImage(image);
					addTexture(TextFormat("%s-%i", name.c_str(), imageScale), texture);
					UnloadImage(image);
				}
			}
		}
	} else {
		auto dirList = LoadDirectoryFiles("fonts/");
		for (int i = 0; i < dirList.count; i++) {
			auto path = dirList.paths[i];
			addFontFromFile(path, 13);
		}
	}
}

ResourceService::~ResourceService() = default;

void ResourceService::addTexture(const std::string &id, Texture2D texture) {
	printf("%s was added \n", id.c_str());
	textures[id] = texture;
}

void ResourceService::addTextureFromFile(const std::string &filePath) {
	Texture2D text = LoadTexture(filePath.c_str());
	std::string newId = GetFileNameWithoutExt(filePath.c_str());
	addTexture(newId, text);
}

Texture2D ResourceService::getTexture(const std::string &id) { return textures.at(id); }

bool ResourceService::textureExists(const std::string &id) { return textures.count(id) > 0; }

void ResourceService::addFont(const std::string &id, Font font) { fonts[id] = font; }

void ResourceService::addFontFromFile(const std::string &filePath, int fontSize) {
	Font font = LoadFontEx(filePath.c_str(), fontSize, nullptr, 256);
	addFont(GetFileNameWithoutExt(filePath.c_str()), font);
}

Font ResourceService::getFont(const std::string &id) { return fonts.at(id); }

bool ResourceService::fontExists(const std::string &id) { return fonts.count(id) > 0; }

void ResourceService::unload() const {
	for (const auto &[name, texture] : textures) {
		if (IsTextureValid(texture)) {
			UnloadTexture(texture);
		}
	}

	for (const auto &[id, font] : fonts) {
		if (IsFontValid(font)) {
			UnloadFont(font);
		}
	}
}
