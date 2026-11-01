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

using namespace editor;

ProjectScreen::ProjectScreen() : edui::Container() {
	auto topBar = std::make_shared<ProjectTopBar>();
	topBar->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT + (INTERFACE_SPACING * 2)});
	add(topBar);
}
