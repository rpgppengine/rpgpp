#ifndef _EDITOR_WIDGETS_PROJECTTOPBAR_H
#define _EDITOR_WIDGETS_PROJECTTOPBAR_H

#include <memory>

#include "edui/horizontalContainer.hpp"
#include "edui/label.hpp"
namespace editor {
struct ProjectTopBar : public edui::HorizontalContainer {
	ProjectTopBar();

	void updateInfo();

private:
	std::shared_ptr<edui::Label> projectTitleLabel = nullptr;
};
}  // namespace editor

#endif
