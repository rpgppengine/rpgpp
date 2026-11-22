#include "widgets/viewerPanel.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/container.hpp"
#include "edui/fileTab.hpp"
#include "edui/fileTabBar.hpp"
#include "edui/gui.hpp"
#include "raylib.h"
#include "resource.hpp"
#include "services/interface.hpp"
#include "widgets/filePanels/filePanel.hpp"

using namespace editor;

ViewerPanel::ViewerPanel() : edui::Container() {
	render->border = 0;

	content = std::make_shared<edui::Container>();
	content->render->border = 0;
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

		std::unique_ptr<Resource> ptr;
		factory.load(fullPath, ptr);
		ptr->loadFile();

		std::string fileName = GetFileName(fullPath.c_str());
		auto fileContent = tabsBar->addItem(fileName, category.iconId);
		fileContent->render->border = 0;
		int refer = fileContent->referId;

		auto panel = factory.createPanel();
		panel->referId = 1;
		panel->setSize({1, 0}, {1, 0});
		fileContent->add(panel);

		panel->setSourcePath(fullPath);
		panel->setResource(std::move(ptr));
		panel->init();
		panel->dirty.connect([this, refer](bool dirty) {
			auto &tab = tabsBar->getWidgetById(refer)->as<edui::FileTab>();
			tab.setDirty(dirty);
			tabsBar->updateWidgets();
		});

		tabsBar->unfocused();
	}
}

std::shared_ptr<FilePanel> ViewerPanel::getCurrent() {
	std::shared_ptr<FilePanel> res = nullptr;
	auto ptr = tabsBar->getPage();
	if (ptr != nullptr) {
		res = std::dynamic_pointer_cast<FilePanel>(ptr->getWidgetById(1));
	}
	return res;
}
