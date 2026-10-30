#include "services/resources.hpp"

#include <stdexcept>

#include "editor.hpp"
#include "edui/fileChooser.hpp"
#include "edui/gui.hpp"
#include "raylib.h"
#include "utils/recentProjects.hpp"
#include "widgets/windows/newProjectWindow.hpp"

using namespace editor;

Resources::Resources() { loadResources(); }

void Resources::update() { openedProject.update(); }

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

Project &Resources::getProject() { return openedProject; }

RecentProjects &Resources::getRecentProjects() { return recentProjects; }

void Resources::onNewProject() {
	auto window = std::make_shared<NewProjectWindow>();
	Editor::getInstance().getInterface().getEdui().add(window, edui::EDUI_MAX_LAYERS - 2);
	Editor::getInstance().getInterface().getEdui().deferFlag = true;
}

void Resources::onOpenProject() {
	auto fileDialog = std::make_shared<edui::FileChooser>();
	fileDialog->centerLayout();
	fileDialog->addCategory("RPG++ Project", {".rpgpp"}, ICON_PRIORITY);

	fileDialog->onFileChosen.connect([](std::string filePath) {
		if (FileExists(filePath.c_str())) {
			Editor::getInstance().getResources().getProject().openFromPath(filePath);
		}
	});

	Editor::getInstance().getInterface().getEdui().add(fileDialog, edui::EDUI_MAX_LAYERS - 1);
	Editor::getInstance().getInterface().getEdui().deferFlag = true;
}
