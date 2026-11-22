#ifndef _EDITOR_WIDGETS_FILEPANEL_H
#define _EDITOR_WIDGETS_FILEPANEL_H

#include <memory>

#include "edui/container.hpp"
#include "edui/fileTabBar.hpp"
#include "utils/commandHistory.hpp"
#include "widgets/filePanels/filePanel.hpp"

namespace editor {
class ViewerPanel : public edui::Container {
public:
	ViewerPanel();
	void openFile(const std::string &fullPath);
	std::shared_ptr<FilePanel> getCurrent();

protected:
	std::shared_ptr<edui::Container> content;
	std::shared_ptr<edui::FileTabBar> tabsBar;
	CommandHistory history;
};
}  // namespace editor

#endif
