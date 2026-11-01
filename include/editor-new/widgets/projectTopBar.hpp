#ifndef _EDITOR_WIDGETS_PROJECTTOPBAR_H
#define _EDITOR_WIDGETS_PROJECTTOPBAR_H

#include <memory>

#include "edui/horizontalContainer.hpp"
#include "edui/label.hpp"
#include "services/interface.hpp"
namespace editor {
const int TOPBAR_LABEL_SIZE = 240;
const int TOPBAR_HEIGHT = edui::EDUI_SECONDARY_HEIGHT + (INTERFACE_SPACING * 2);

struct ProjectTopBar : public edui::HorizontalContainer {
	ProjectTopBar();

	void updateInfo();

private:
	std::shared_ptr<edui::Label> projectTitleLabel = nullptr;
};
}  // namespace editor

#endif
