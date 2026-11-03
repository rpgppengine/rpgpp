#ifndef _EDITOR_WIDGETS_RESOURCESPANEL_H
#define _EDITOR_WIDGETS_RESOURCESPANEL_H

#include <filesystem>
#include <memory>
#include <string>

#include "edui/childWindow.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"

namespace editor {
const int RESOURCESPANEL_OFFSET = edui::EDUI_SECONDARY_HEIGHT + INTERFACE_SPACING;

enum ResourceContextMenuItem {
	RESOURCE_PASTE = -1,
	RESOURCE_COPY,
	RESOURCE_COPY_PATH,
	RESOURCE_RENAME,
	RESOURCE_DELETE
};

struct ResourcesPanel : public edui::ChildWindow {
	ResourcesPanel();

private:
	std::string relativeDir = "";
	std::filesystem::path path = "";
	std::shared_ptr<edui::VerticalContainer> filesList = nullptr;
	std::shared_ptr<edui::TextBox> pathBox = nullptr;
	void changePath(std::filesystem::path newPath);
	void refresh();
	void addItem(const std::string &path);
	void showContextMenu(const std::string &filePath);
	void addDirectory(const std::string &title);
	void pasteItem();
	void renameItem(const std::string &fullPath, const std::string &newName);
	void deleteItem(const std::string &fullPath);
};
}  // namespace editor

#endif
