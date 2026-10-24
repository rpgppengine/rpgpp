#include "widgets/recentProjectsList.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/gui.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/rliconsarr.hpp"
#include "raylib.h"

using namespace editor;

RecentProjectsList::RecentProjectsList() {
	auto recentProjects = Editor::getInstance().getResources().getRecentProjects();
	for (auto &item : recentProjects.getRecentProjects()) {
		addItem(item);
	}
}

void RecentProjectsList::addItem(const std::string &file) {
	if (!FileExists(file.c_str())) {
		return;
	}

	auto button = std::make_shared<edui::IconTextButton>();
	button->render->border = 0;
	button->iconId = ICON_FILE;
	button->setText(GetFileName(file.c_str()));
	button->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});

	add(button);
}
