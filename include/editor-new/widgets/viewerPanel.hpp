#ifndef _EDITOR_WIDGETS_FILEPANEL_H
#define _EDITOR_WIDGETS_FILEPANEL_H

#include <memory>

#include "edui/container.hpp"
#include "edui/fileTabBar.hpp"

namespace editor {
class ViewerPanel : public edui::Container {
public:
	ViewerPanel();
	void openFile(const std::string &fullPath);

private:
	std::shared_ptr<edui::Container> content;
	std::shared_ptr<edui::FileTabBar> tabsBar;
};
}  // namespace editor

#endif
