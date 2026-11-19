#ifndef _EDITOR_WIDGETS_SCREENS_PROJECTSCREEN_H
#define _EDITOR_WIDGETS_SCREENS_PROJECTSCREEN_H

#include <memory>

#include "edui/container.hpp"
#include "widgets/viewerPanel.hpp"
namespace editor {
struct ProjectScreen : public edui::Container {
	ProjectScreen();
	std::shared_ptr<ViewerPanel> getViewer();

private:
	std::shared_ptr<ViewerPanel> viewerPanel = nullptr;
};
}  // namespace editor

#endif
