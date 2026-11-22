#include "edui/fileInput.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/fileChooser.hpp"
#include "edui/gui.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "raylib.h"

using namespace edui;

static const std::string builtinTranslation = "Widgets.FileChooser";

FileInput::FileInput() : Container() {
	render = std::make_unique<FileInputRender>();
	button = std::make_shared<Button>();
	button->setSize({1, static_cast<int>(-EDUI_SECONDARY_HEIGHT)}, {1, 0});
	button->render->as<LabelRender>().vertAlign = VerticalAlignment::TEXT_CENTER;
	button->onClicked.connect([this] { onButtonClicked(); });
	add(button);

	auto removeButton = std::make_shared<IconButton>();
	removeButton->setPosition({1, static_cast<int>(-EDUI_SECONDARY_HEIGHT)}, {0, 0});
	removeButton->setSize({0, static_cast<int>(EDUI_SECONDARY_HEIGHT)}, {1, 0});
	removeButton->iconId = ICON_FILE_DELETE;
	removeButton->onClicked.connect([this] { setEmpty(); });
	add(removeButton);

	setEmpty();
}

void FileInput::translate() {
	Container::translate();
	if (!file.filled) {
		if (!dirSelection) {
			button->setText(getTranslation(builtinTranslation, "Select file"));
		} else {
			button->setText(getTranslation(builtinTranslation + ".Folder", "Select file"));
		}
	}
}

void FileInput::setValue(const FileOption &newValue) {
	this->file = newValue;
	setFromPath(file.fullPath);
}

FileOption FileInput::getValue() { return file; }

void FileInput::setEmpty() {
	file.filled = false;
	translate();
	fileValueEmptied.invoke();
}

void FileInput::setFromPath(const std::string &fullPath) {
	bool isFile = IsPathFile(fullPath.c_str()) && !dirSelection;
	bool isDirectory = DirectoryExists(fullPath.c_str()) && dirSelection;
	if (isFile || isDirectory) {
		auto oldFile = file;

		file.fullPath = fullPath;
		file.filled = true;
		button->setText(GetFileName(fullPath.c_str()));

		onValueChanged.invoke(oldFile, file);
	}
}

void FileInput::addCategory(const std::string &title, std::vector<std::string> extensions, int iconId) {
	FileCategory category;
	category.title = title;
	for (int i = 0;
		 i < (extensions.size() < FILE_CATEGORY_MAX_EXTENSIONS ? extensions.size() : FILE_CATEGORY_MAX_EXTENSIONS);
		 i++) {
		category.extensions[i] = extensions[i];
	}
	category.iconId = iconId;

	categories.push_back(category);
}

void FileInput::addImagesCategory() { addCategory("Images", {".png", ".jpg"}, ICON_FILETYPE_IMAGE); }

void FileInput::addTextCategory() { addCategory("Text file", {".txt"}, ICON_FILETYPE_TEXT); }

void FileInput::addAudioCategory() { addCategory("Audio", {".wav", ".mp4", ".ogg"}, ICON_FILETYPE_AUDIO); }

void FileInput::addFontCategory() { addCategory("Font", {".ttf", ".otf"}, ICON_FILETYPE_FONT); }

void FileInput::setDirSelection(bool val) {
	this->dirSelection = val;
	translate();
}

void FileInput::lockBasePath(const std::filesystem::path &basePath) {
	this->basePath = basePath;
	useBasePath = true;
}

void FileInput::onButtonClicked() {
	auto fileDialog = std::make_shared<FileChooser>();
	fileDialog->setSize({0, FILE_CHOOSER_WIDTH}, {0, FILE_CHOOSER_HEIGHT});
	fileDialog->onFileChosen.connect([this](std::string fullPath) { setFromPath(fullPath); });
	fileDialog->clearCategories();
	fileDialog->setDirSelection(dirSelection);
	if (useBasePath) {
		fileDialog->lockBasePath(basePath);
	}
	for (auto &category : categories) {
		fileDialog->addCategory(category);
	}
	fileDialog->centerLayout();
	gui->add(fileDialog, layerId + 1);
}
