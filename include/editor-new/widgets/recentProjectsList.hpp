#ifndef _EDITOR_WIDGETS_RECENTPROJECTSLIST_H
#define _EDITOR_WIDGETS_RECENTPROJECTSLIST_H

#include <string>

#include "edui/verticalContainer.hpp"

namespace editor {
struct RecentProjectsList : public edui::VerticalContainer {
	RecentProjectsList();
	void update();

private:
	void addItem(const std::string &file);
	void queueOpen(const std::string &file);

	bool queued = false;
	std::string path = "";
};
}  // namespace editor

#endif
