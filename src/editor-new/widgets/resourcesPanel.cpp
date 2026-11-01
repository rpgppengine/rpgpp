#include "widgets/resourcesPanel.hpp"

#include <cstdio>
#include <filesystem>
#include <memory>

#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "icons.hpp"
#include "raylib.h"
#include "services/interface.hpp"

using namespace editor;

ResourcesPanel::ResourcesPanel() : edui::ChildWindow() {
	translationId = "Project.Resources";

	draggable = false;
	render->padding = INTERFACE_SPACING;
	hideCloseButton();

	filesList = std::make_shared<edui::VerticalContainer>();
	filesList->setPosition({0, 0}, {0, RESOURCESPANEL_FILESLIST_OFFSET});
	filesList->setSize({1, 0}, {1, -RESOURCESPANEL_FILESLIST_OFFSET});
	add(filesList);

	auto upButton = std::make_shared<edui::IconButton>();
	upButton->iconId = ICON_ARROW_UP;
	upButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {0, edui::EDUI_SECONDARY_HEIGHT});
	upButton->onClicked.connect([this] { changePath(path.parent_path()); });
	add(upButton);

	pathBox = std::make_shared<edui::TextBox>();
	pathBox->setPosition({0, RESOURCESPANEL_FILESLIST_OFFSET}, {0, 0});
	pathBox->setSize({1, -RESOURCESPANEL_FILESLIST_OFFSET}, {0, edui::EDUI_SECONDARY_HEIGHT});
	pathBox->onUnfocused.connect([this] {
		std::string text = this->pathBox->getValue();
		changePath(text);
	});
	add(pathBox);

	refresh();
};

void ResourcesPanel::changePath(std::filesystem::path newPath) {
	std::filesystem::path finalPath = Editor::getInstance().getResources().getProject().getBasePath();
	finalPath.append(newPath.c_str());

	if (!std::filesystem::exists(finalPath)) {
		pathBox->setText(path.c_str());
		return;
	}

	path = newPath;

	refresh();
}

void ResourcesPanel::refresh() {
	filesList->deleteChildren();

	std::filesystem::path finalPath = Editor::getInstance().getResources().getProject().getBasePath();
	finalPath.append(path.c_str());

	if (!std::filesystem::exists(finalPath)) {
		pathBox->setText(path.c_str());
		return;
	}

	pathBox->setText(path.c_str());

	auto files = LoadDirectoryFiles(finalPath.c_str());

	for (int i = 0; i < files.count; i++) {
		std::string filePath = files.paths[i];
		addItem(filePath);
	}

	UnloadDirectoryFiles(files);
}

void ResourcesPanel::addItem(const std::string &path) {
	if (!FileExists(path.c_str()) && !DirectoryExists(path.c_str())) return;

	auto button = std::make_shared<edui::IconTextButton>();
	button->render->border = 0;
	button->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	button->setText(GetFileName(path.c_str()));

	if (DirectoryExists(path.c_str())) {
		button->iconId = ICON_FOLDER;
		button->onClicked.connect([this, path] {
			std::string dirName = GetFileName(path.c_str());
			changePath(this->path.append(dirName));
		});
	} else {
		button->iconId = ICON_FILE;
	}

	filesList->add(button);
}
