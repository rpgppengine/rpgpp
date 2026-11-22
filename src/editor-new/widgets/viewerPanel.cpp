#include "widgets/viewerPanel.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/container.hpp"
#include "edui/fileTabBar.hpp"
#include "edui/gui.hpp"
#include "edui/tabPage.hpp"
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

	tabsBar->onActiveTabChanged.connect([this](int previous, int newId) {
		/*
		auto containerPrevious = tabsBar->getWidgetById(previous);
		FilePanel &panelPrevious = containerPrevious->as<edui::Container>().getWidgetById(1)->as<FilePanel>();
		panelPrevious.onClose();

		auto container = tabsBar->getWidgetById(newId);
		FilePanel &panel = container->as<edui::Container>().getWidgetById(1)->as<FilePanel>();
		panel.onOpen();
		*/
	});

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

		auto panel = factory.createPanel();
		panel->referId = 1;
		panel->setSize({1, 0}, {1, 0});
		fileContent->add(panel);

		panel->setSourcePath(fullPath);
		panel->setResource(std::move(ptr));
		panel->init();

		std::weak_ptr<FilePanel> weakPanel = panel;
		fileContent->onClose.connect([weakPanel] {
			if (auto sharedPanel = weakPanel.lock()) {
				sharedPanel->onClose();
			}
		});

		tabsBar->unfocused();
	}
}
