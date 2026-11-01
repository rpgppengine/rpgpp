#include "widgets/screens/projectScreen.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "services/interface.hpp"
#include "utils/editorIcons.hpp"
#include "widgets/projectTopBar.hpp"
#include "widgets/resourcesPanel.hpp"

using namespace editor;

ProjectScreen::ProjectScreen() : edui::Container() {
	auto topBar = std::make_shared<ProjectTopBar>();
	topBar->setSize({1, 0}, {0, TOPBAR_HEIGHT});
	add(topBar);

	auto resourcesPanel = std::make_shared<ResourcesPanel>();
	resourcesPanel->setPosition({0, 0}, {0, TOPBAR_HEIGHT});
	resourcesPanel->setSize({0, TOPBAR_LABEL_SIZE}, {1, -TOPBAR_HEIGHT});
	add(resourcesPanel);
}
