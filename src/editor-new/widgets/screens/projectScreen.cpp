#include "widgets/screens/projectScreen.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/label.hpp"
#include "edui/rliconsarr.hpp"
#include "services/interface.hpp"

using namespace editor;

ProjectScreen::ProjectScreen() : edui::Container() {
	auto topBar = std::make_shared<edui::HorizontalContainer>();
	topBar->render->padding = INTERFACE_SPACING;
	topBar->render->as<edui::HorizontalContainerRender>().space = INTERFACE_SPACING;
	topBar->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT + (INTERFACE_SPACING * 2)});
	add(topBar);

	auto projectTitleLabel = std::make_shared<edui::Label>();
	projectTitleLabel->render->border = 0;
	projectTitleLabel->setSize({0, 240}, {1, 0});
	projectTitleLabel->setText(Editor::getInstance().getResources().getProject().getTitle());
	topBar->add(projectTitleLabel);

	auto buildButton = std::make_shared<edui::IconButton>();
	buildButton->iconId = ICON_NONE;
	buildButton->translationId = "Project.Build";
	buildButton->setTooltip();
	buildButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topBar->add(buildButton);

	auto playButton = std::make_shared<edui::IconButton>();
	playButton->iconId = ICON_PLAYER_PLAY;
	playButton->translationId = "Project.Play";
	playButton->setTooltip();
	playButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});

	playButton->onClicked.connect([] { Editor::getInstance().getResources().getProject().spawnGame(); });

	topBar->add(playButton);
}
