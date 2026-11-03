#include "widgets/resourcesPanel.hpp"

#include <filesystem>
#include <memory>

#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/contextMenu.hpp"
#include "edui/gui.hpp"
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/label.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "icons.hpp"
#include "raylib.h"
#include "services/interface.hpp"
#include "widgets/windows/deleteConfirmWindow.hpp"
#include "widgets/windows/newFolderWindow.hpp"
#include "widgets/windows/renameResourceWindow.hpp"

using namespace editor;

ResourcesPanel::ResourcesPanel() : edui::ChildWindow() {
	translationId = "Project.Resources";

	draggable = false;
	render->padding = INTERFACE_SPACING;
	hideCloseButton();

	filesList = std::make_shared<edui::VerticalContainer>();
	filesList->setPosition({0, 0}, {0, RESOURCESPANEL_OFFSET * 2});
	filesList->setSize({1, 0}, {1, -(RESOURCESPANEL_OFFSET * 2)});
	add(filesList);

	auto upButton = std::make_shared<edui::IconButton>();
	upButton->iconId = ICON_ARROW_UP;
	upButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {0, edui::EDUI_SECONDARY_HEIGHT});
	upButton->onClicked.connect([this] { changePath(path.parent_path()); });
	add(upButton);

	pathBox = std::make_shared<edui::TextBox>();
	pathBox->setPosition({0, RESOURCESPANEL_OFFSET}, {0, 0});
	pathBox->setSize({1, -(RESOURCESPANEL_OFFSET * 2)}, {0, edui::EDUI_SECONDARY_HEIGHT});
	pathBox->onUnfocused.connect([this] {
		std::string text = this->pathBox->getValue();
		changePath(text);
	});
	add(pathBox);

	auto refreshButton = std::make_shared<edui::IconButton>();
	refreshButton->iconId = ICON_RESTART;
	refreshButton->setPosition({1, -edui::EDUI_SECONDARY_HEIGHT}, {0, 0});
	refreshButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {0, edui::EDUI_SECONDARY_HEIGHT});
	refreshButton->onClicked.connect([this] { this->refresh(); });
	add(refreshButton);

	auto newFolderButton = std::make_shared<edui::IconButton>();
	newFolderButton->iconId = ICON_FOLDER_ADD;
	newFolderButton->setPosition({0, 0}, {0, RESOURCESPANEL_OFFSET});
	newFolderButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {0, edui::EDUI_SECONDARY_HEIGHT});
	newFolderButton->onClicked.connect([this] {
		auto newFolderWindow = std::make_shared<NewFolderWindow>();
		newFolderWindow->onConfirmed.connect([this](std::string folderTitle) { addDirectory(folderTitle); });
		Editor::getInstance().getInterface().showTopWindow(newFolderWindow);
	});
	add(newFolderButton);

	auto newResourceButton = std::make_shared<edui::IconTextButton>();
	newResourceButton->translationId = "Project.Resources.New";
	newResourceButton->render->as<edui::IconTextButtonRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;
	newResourceButton->iconId = ICON_FILE_ADD;
	newResourceButton->setText("New Resource");
	newResourceButton->setPosition({0, RESOURCESPANEL_OFFSET}, {0, RESOURCESPANEL_OFFSET});
	newResourceButton->setSize({1, -RESOURCESPANEL_OFFSET}, {0, edui::EDUI_SECONDARY_HEIGHT});
	add(newResourceButton);

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
	button->onRightClicked.connect([this, path] { showContextMenu(path); });

	filesList->add(button);
}

void ResourcesPanel::showContextMenu(const std::string &filePath) {
	bool fileClipAvailable = Editor::getInstance().getResources().fileClipboardAvailable();
	auto contextMenu = std::make_shared<edui::ContextMenu>();

	if (fileClipAvailable) {
		contextMenu->addItem("Paste");
	}

	contextMenu->addItem("Copy");
	contextMenu->addItem("Copy full path");
	contextMenu->addItem("Rename");
	contextMenu->addItem("Delete");
	contextMenu->moveToMouse();
	Editor::getInstance().getInterface().getEdui().add(contextMenu, edui::EDUI_MAX_LAYERS - 1);

	contextMenu->onItemClicked.connect([this, filePath, fileClipAvailable](std::string item, int itemIdx) {
		Editor::getInstance().getInterface().getEdui().deferFlag = true;
		if (fileClipAvailable) {
			itemIdx--;
		}
		switch (itemIdx) {
			case RESOURCE_PASTE:
				pasteItem();
				break;
			case RESOURCE_COPY:
				Editor::getInstance().getResources().setFileClipboard(filePath);
				break;
			case RESOURCE_COPY_PATH:
				SetClipboardText(filePath.c_str());
				break;
				break;
			case RESOURCE_RENAME: {
				auto window = std::make_shared<RenameResourceWindow>();
				window->setTitleInput(GetFileName(filePath.c_str()));
				window->onConfirmed.connect(
					[this, filePath](std::string newName) { this->renameItem(filePath, newName); });
				Editor::getInstance().getInterface().showTopWindow(window);
			} break;
			case RESOURCE_DELETE: {
				auto window = std::make_shared<DeleteConfirmWindow>();
				window->onOkPressed.connect([this, filePath] { this->deleteItem(filePath); });
				Editor::getInstance().getInterface().showTopWindow(window);
			} break;
			default:
				break;
		}
	});
}

void ResourcesPanel::addDirectory(const std::string &title) {
	std::filesystem::path finalPath = Editor::getInstance().getResources().getProject().getBasePath();
	finalPath.append(path.c_str());
	if (!std::filesystem::exists(finalPath)) {
		pathBox->setText(path.c_str());
		return;
	}

	finalPath.append(title);

	int result = MakeDirectory(finalPath.c_str());
	if (result != 0) {
		Editor::getInstance().getInterface().showErrorMessage(TextFormat("MakeDirectory(): %i", result));
	} else {
		this->refresh();
	}
}

void ResourcesPanel::pasteItem() {
	if (Editor::getInstance().getResources().fileClipboardAvailable()) {
		std::string filePath = Editor::getInstance().getResources().getFileClipboard();

		std::filesystem::path destPath = Editor::getInstance().getResources().getProject().getBasePath();
		destPath.append(path.c_str());
		destPath.append(GetFileName(filePath.c_str()));

		int result = FileCopy(filePath.c_str(), destPath.c_str());
		/*
		if (result != 0) {
			Editor::getInstance().getInterface().showErrorMessage(TextFormat("FileCopy(): %i", result));
		} else {
			this->refresh();
		}
		*/
		this->refresh();
	}
}

void ResourcesPanel::renameItem(const std::string &fullPath, const std::string &newName) {
	int result = FileRename(fullPath.c_str(), newName.c_str());
	if (result != 0) {
		Editor::getInstance().getInterface().showErrorMessage(TextFormat("FileRename(): %i", result));
	} else {
		this->refresh();
	}
}

void ResourcesPanel::deleteItem(const std::string &fullPath) {
	int result = FileRemove(fullPath.c_str());
	if (result != 0) {
		Editor::getInstance().getInterface().showErrorMessage(TextFormat("FileRemove(): %i", result));
	} else {
		this->refresh();
	}
}
