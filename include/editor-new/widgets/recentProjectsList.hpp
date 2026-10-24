#ifndef _EDITOR_WIDGETS_RECENTPROJECTSLIST_H
#define _EDITOR_WIDGETS_RECENTPROJECTSLIST_H

#include <string>

#include "edui/verticalContainer.hpp"

namespace editor {
struct RecentProjectsList : public edui::VerticalContainer {
	RecentProjectsList();

private:
	void addItem(const std::string &file);
};
}  // namespace editor

#endif
