#include "widgets/screens/projectScreen.hpp"

#include <memory>

#include "edui/container.hpp"
#include "services/interface.hpp"
#include "widgets/projectTopBar.hpp"
#include "widgets/resourcesPanel.hpp"
#include "widgets/viewerPanel.hpp"

using namespace editor;

ProjectScreen::ProjectScreen() : edui::Container() {
	auto topBar = std::make_shared<ProjectTopBar>();
	topBar->setSize({1, 0}, {0, TOPBAR_HEIGHT});
	add(topBar);

	const int RESOURCES_WIDTH = TOPBAR_LABEL_SIZE;
	const int BASE_Y = TOPBAR_HEIGHT + INTERFACE_SPACING;
	const int LEFT_HEIGHT = TOPBAR_HEIGHT + (INTERFACE_SPACING * 2);

	auto resourcesPanel = std::make_shared<ResourcesPanel>();
	resourcesPanel->setPosition({0, INTERFACE_SPACING}, {0, BASE_Y});
	resourcesPanel->setSize({0, RESOURCES_WIDTH}, {1, -LEFT_HEIGHT});
	add(resourcesPanel);

	viewerPanel = std::make_shared<ViewerPanel>();
	viewerPanel->setPosition({0, RESOURCES_WIDTH + (INTERFACE_SPACING * 2)}, {0, BASE_Y});
	viewerPanel->setSize({1, -(RESOURCES_WIDTH + (INTERFACE_SPACING * 3))}, {1, -LEFT_HEIGHT});
	add(viewerPanel);
}

std::shared_ptr<ViewerPanel> ProjectScreen::getViewer() { return viewerPanel; }
