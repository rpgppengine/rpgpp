#ifndef _EDITOR_WIDGETS_RESOURCESPANEL_H
#define _EDITOR_WIDGETS_RESOURCESPANEL_H

#include <filesystem>
#include <memory>

#include "edui/childWindow.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"

namespace editor {
const int RESOURCESPANEL_FILESLIST_OFFSET = edui::EDUI_SECONDARY_HEIGHT + INTERFACE_SPACING;

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
};
}  // namespace editor

#endif
