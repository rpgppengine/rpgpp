#include "edui/fileChooser.hpp"

#include <memory>

#include "edui/confirmDialog.hpp"
#include "edui/dropdown.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/textBox.hpp"
#include "edui/toggleButton.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

static const std::string builtinTranslation = "Widgets.FileChooser";

FileChooser::FileChooser() : ConfirmDialog() {
	render = std::make_unique<FileChooserRender>();
	render->padding = 4;

	setText("");
	setTitle("Choose file");

	initTopContainer();
	initListContainer();

	filterBox = std::make_shared<edui::Dropdown>();
	filterBox->setSize({0.5f, 0}, {1, 0});
	filterBox->onValueChangedT.connect([this](DropdownValue old, DropdownValue newValue) {
		updateFileFilter();
		setDirectory(dirPath);
	});

	getOkButton().disabled = true;
	getOkButton().onClicked.connect([this] { onFileChosen.invoke(chosenPath); });
}

void FileChooser::applyTheme(const mINI::INIStructure &struc) {
	ConfirmDialog::applyTheme(struc);
	filterBox->applyTheme(struc);
	filterBox->unfocused();
}

void FileChooser::translate() {
	ConfirmDialog::translate();
	filterBox->translate();
	setTitle(getTranslation(builtinTranslation, "Select file"));
}

void FileChooser::initTopContainer() {
	int topHeight = static_cast<int>((EDUI_SECONDARY_HEIGHT));
	auto topContainer = std::make_shared<edui::HorizontalContainer>();
	topContainer->render->as<HorizontalContainerRender>().space = 4;
	topContainer->render->border = 0;
	topContainer->setSize({1, 0}, {0, topHeight});

	auto screenRect = Gui::instance->getScreenRect();
	calcRect(screenRect);

	updateContentRect();
	topContainer->calcRect(renderRect);

	auto goUpButton = std::make_shared<edui::IconButton>();
	goUpButton->iconId = ICON_ARROW_UP;
	goUpButton->setSize({0, topHeight}, {1, 0});
	goUpButton->onClicked.connect([this] { setDirectory(GetPrevDirectoryPath(dirPath.c_str())); });
	topContainer->add(goUpButton);

	pathBox = std::make_shared<edui::TextBox>();
	pathBox->setSize({1, -(topHeight + 4)}, {1, 0});
	pathBox->onUnfocused.connect([this] { setDirectory(this->pathBox->getValue()); });
	topContainer->add(pathBox);

	add(topContainer);
}

void FileChooser::initListContainer() {
	int topHeight = static_cast<int>((EDUI_SECONDARY_HEIGHT));

	listContainer = std::make_shared<edui::VerticalContainer>();
	float posOffset = topHeight + 4;
	float sizeOffset = -posOffset;
	listContainer->setPosition({0, 0}, {0, static_cast<int>(posOffset)});
	listContainer->setSize({1, 0}, {1, static_cast<int>(sizeOffset)});
	add(listContainer);
}

void FileChooser::addDefaultCategories() {
	addCategory("Images", {".png", ".jpg"}, ICON_FILETYPE_IMAGE);
	addCategory("Text file", {".txt"}, ICON_FILETYPE_TEXT);
	addCategory("Audio", {".wav", ".mp4", ".ogg"}, ICON_FILETYPE_AUDIO);
}

void FileChooser::update() {
	if (changeQueued) {
		setDirectory(queuedPath);
		changeQueued = false;
	}

	ConfirmDialog::update();

	auto &rend = render->as<MessageBoxRender>();
	Rectangle paddingFooter = paddingRect(footerRect, rend.padding);
	filterBox->calcRect(paddingFooter);
	filterBox->update();

	std::shared_ptr<Widget> widgetBox = filterBox;
	edui::processPseudoChild(gui, widgetBox);
}

void FileChooser::draw() {
	ConfirmDialog::draw();

	filterBox->draw();
}

void FileChooser::onAdded() {
	ConfirmDialog::onAdded();
	filterBox->render->font = this->render->font;

	updateFileFilter();

	dirPath = GetApplicationDirectory();
	setDirectory(dirPath);
}

void FileChooser::addCategory(const std::string &title, std::vector<std::string> extensions, int iconId) {
	FileCategory category;
	category.title = title;
	for (int i = 0;
		 i < (extensions.size() < FILE_CATEGORY_MAX_EXTENSIONS ? extensions.size() : FILE_CATEGORY_MAX_EXTENSIONS);
		 i++) {
		category.extensions[i] = extensions[i];
	}
	category.iconId = iconId;

	categories.push_back(category);

	filterBox->addItem(title);
}

void FileChooser::addCategory(FileCategory &category) {
	categories.push_back(category);
	filterBox->addItem(category.title);
}

void FileChooser::clearCategories() {
	categories.clear();
	filterBox->clear();
}

void FileChooser::setDirectory(std::string fullPath) {
	if (!DirectoryExists(fullPath.c_str())) {
		pathBox->setText(dirPath);
		return;
	}

	std::string last = fullPath.end().base();
	if (fullPath.back() == '/') {
		fullPath.pop_back();
	}

	getOkButton().disabled = true;

	dirPath = fullPath;
	pathBox->setText(fullPath);

	listContainer->deleteChildren();

	auto dirFiles = LoadDirectoryFilesEx(fullPath.c_str(), fileFilter.c_str(), false);

	for (auto i = 0; i < dirFiles.count; i++) {
		std::string filePath = dirFiles.paths[i];

		auto fileButton = std::make_shared<edui::ToggleButton>();
		fileButton->render->border = 0;
		fileButton->iconId = ICON_FILE;
		fileButton->setSize({1, 0}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
		fileButton->setText(GetFileName(filePath.c_str()));

		if (DirectoryExists(filePath.c_str())) {
			fileButton->iconId = ICON_FOLDER;
			fileButton->onClicked.connect([this, filePath] {
				printf("%s \n", filePath.c_str());
				queueDirectoryChange(filePath);
			});
		} else {
			int iconId = ICON_FILE;
			for (auto &cat : categories) {
				for (auto &ext : cat.extensions) {
					if (IsFileExtension(filePath.c_str(), ext.c_str())) {
						iconId = cat.iconId;
					}
				}
			}
			fileButton->iconId = iconId;

			std::weak_ptr<edui::ToggleButton> weakButton = fileButton;
			fileButton->onToggled.connect([this, weakButton, filePath](bool newValue) {
				if (newValue) {
					if (auto sharedPrevious = chosenButton.lock()) {
						sharedPrevious->setInactive();
						sharedPrevious->unfocused();
					}

					this->chosenButton = weakButton;
					this->chosenPath = filePath;
					getOkButton().disabled = false;
				}
			});
		}

		listContainer->add(fileButton);
	}

	UnloadDirectoryFiles(dirFiles);

	listContainer->updateWidgets();
	listContainer->resetScroll();
}

void FileChooser::queueDirectoryChange(const std::string &fullPath) {
	changeQueued = true;
	queuedPath = fullPath;
}

void FileChooser::updateFileFilter() {
	fileFilter = "DIRS*";
	int index = filterBox->getValue().idx;
	FileCategory &category = categories[index];
	for (auto &ext : category.extensions) {
		fileFilter = fileFilter.append(";");
		fileFilter = fileFilter.append(ext);
	}
}
