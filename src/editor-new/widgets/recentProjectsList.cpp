#include "widgets/recentProjectsList.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/gui.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/verticalContainer.hpp"
#include "raylib.h"

using namespace editor;

RecentProjectsList::RecentProjectsList() {
	auto recentProjects = Editor::getInstance().getResources().getRecentProjects();
	for (auto &item : recentProjects.getRecentProjects()) {
		addItem(item);
	}
}

void RecentProjectsList::update() {
	edui::VerticalContainer::update();
	if (queued) {
		queued = false;
		Editor::getInstance().getResources().getProject().openFromPath(path);
	}
}

void RecentProjectsList::queueOpen(const std::string &file) {
	queued = true;
	this->path = file;
	Editor::getInstance().getInterface().getEdui().deferFlag = true;
}

void RecentProjectsList::addItem(const std::string &file) {
	if (!FileExists(file.c_str())) {
		return;
	}

	auto button = std::make_shared<edui::IconTextButton>();
	button->render->border = 0;
	button->iconId = ICON_FILE;
	std::string dirName = GetFileName(GetDirectoryPath(file.c_str()));
	button->setText(dirName);
	button->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});

	std::weak_ptr<edui::IconTextButton> weakButton = button;
	button->onClicked.connect([this, file, weakButton] {
		Editor::getInstance().getInterface().getEdui().deferFlag = true;
		Editor::getInstance().getResources().getProject().openFromPath(file);
	});

	add(button);
}
