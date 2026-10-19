#ifndef _EDUI_FILECHOOSER_H
#define _EDUI_FILECHOOSER_H

#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include "edui/confirmDialog.hpp"
#include "edui/dropdown.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/textBox.hpp"
#include "edui/toggleButton.hpp"
#include "edui/verticalContainer.hpp"
namespace edui {
const size_t FILE_CATEGORY_MAX_EXTENSIONS = 5;

struct FileCategory {
	std::string title;
	std::array<std::string, FILE_CATEGORY_MAX_EXTENSIONS> extensions;
	int iconId;
};

struct FileChooserRender : public ConfirmDialogRender {};

struct FileChooser : public ConfirmDialog {
	SignalT<std::string> onFileChosen;

	FileChooser();
	void applyTheme(const mINI::INIStructure &struc) override;
	void translate() override;

	void draw() override;
	void update() override;

	void onAdded() override;

	void setDirectory(std::string fullPath);
	void addCategory(const std::string &title, std::vector<std::string> extensions, int iconId = ICON_FILE);

private:
	std::vector<FileCategory> categories = {};

	std::string dirPath = "";
	std::shared_ptr<edui::VerticalContainer> listContainer = nullptr;
	std::shared_ptr<edui::TextBox> pathBox = nullptr;
	std::shared_ptr<edui::Dropdown> filterBox = nullptr;

	bool changeQueued = false;
	std::string queuedPath = "";
	void queueDirectoryChange(const std::string &fullPath);

	void initTopContainer();
	void initListContainer();
	void addDefaultCategories();

	std::string chosenPath = "";
	std::weak_ptr<edui::ToggleButton> chosenButton;
	std::string fileFilter;
	void updateFileFilter();
};
}  // namespace edui

#endif
