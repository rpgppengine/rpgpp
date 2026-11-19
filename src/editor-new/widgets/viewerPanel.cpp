#include "widgets/viewerPanel.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/container.hpp"
#include "edui/fileTabBar.hpp"
#include "edui/gui.hpp"
#include "icons.hpp"
#include "raylib.h"
#include "services/interface.hpp"

using namespace editor;

ViewerPanel::ViewerPanel() : edui::Container() {
	render->border = 0;

	content = std::make_shared<edui::Container>();
	content->setPosition({0, 0}, {0, (edui::EDUI_SECONDARY_HEIGHT + INTERFACE_SPACING)});
	content->setSize({1, 0}, {1, -(edui::EDUI_SECONDARY_HEIGHT + INTERFACE_SPACING)});
	add(content);

	tabsBar = std::make_shared<edui::FileTabBar>();
	tabsBar->contentBase = content;
	tabsBar->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	add(tabsBar);
}

void ViewerPanel::openFile(const std::string &fullPath) {
	auto &types = Editor::getInstance().getResources().getTypes();
	std::string typeTitle;
	types.getTypeTitleByExtension(GetFileExtension(fullPath.c_str()), &typeTitle);

	if (types.hasFactory(typeTitle)) {
		auto &factory = types.getFactory(typeTitle);
		auto &category = types.getCategory(typeTitle);

		std::string fileName = GetFileName(fullPath.c_str());
		auto fileContent = tabsBar->addItem(fileName, category.iconId);

		auto panel = factory.createPanel();
		panel->setSize({1, 0}, {1, 0});
		fileContent->add(panel);

		tabsBar->unfocused();
	}
}
