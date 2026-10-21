#include "edui/fileValue.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/fileChooser.hpp"
#include "edui/gui.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "edui/rliconsarr.hpp"
#include "raylib.h"

using namespace edui;

static const std::string builtinTranslation = "Widgets.FileChooser";

FileValue::FileValue() : Container() {
	render = std::make_unique<FileValueRender>();
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

void FileValue::translate() {
	Container::translate();
	if (!file.filled) {
		button->setText(getTranslation(builtinTranslation, "Select file"));
	}
}

void FileValue::setValue(const FileOption &newValue) { this->file = newValue; }

FileOption FileValue::getValue() { return file; }

void FileValue::setEmpty() {
	file.filled = false;
	translate();
}

void FileValue::setFromPath(const std::string &fullPath) {
	if (IsPathFile(fullPath.c_str())) {
		file.fullPath = fullPath;
		file.filled = true;
		button->setText(GetFileName(fullPath.c_str()));
	}
}

void FileValue::addCategory(const std::string &title, std::vector<std::string> extensions, int iconId) {
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

void FileValue::addImagesCategory() { addCategory("Images", {".png", ".jpg"}, ICON_FILETYPE_IMAGE); }

void FileValue::addTextCategory() { addCategory("Text file", {".txt"}, ICON_FILETYPE_TEXT); }

void FileValue::addAudioCategory() { addCategory("Audio", {".wav", ".mp4", ".ogg"}, ICON_FILETYPE_AUDIO); }

void FileValue::addFontCategory() { addCategory("Font", {".ttf", ".otf"}, ICON_FILETYPE_FONT); }

void FileValue::onButtonClicked() {
	auto fileDialog = std::make_shared<FileChooser>();
	fileDialog->setSize({0, FILE_CHOOSER_WIDTH}, {0, FILE_CHOOSER_HEIGHT});
	fileDialog->onFileChosen.connect([this](std::string fullPath) { setFromPath(fullPath); });
	fileDialog->clearCategories();
	for (auto &category : categories) {
		fileDialog->addCategory(category);
	}
	gui->add(fileDialog, layerId + 1);
}
