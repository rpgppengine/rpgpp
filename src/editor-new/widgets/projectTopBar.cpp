#include "widgets/projectTopBar.hpp"

#include "editor.hpp"
#include "edui/horizontalContainer.hpp"
#include "services/interface.hpp"

using namespace editor;

ProjectTopBar::ProjectTopBar() : edui::HorizontalContainer() {
	render->padding = INTERFACE_SPACING;
	render->as<edui::HorizontalContainerRender>().space = INTERFACE_SPACING;

	projectTitleLabel = std::make_shared<edui::Label>();
	projectTitleLabel->render->border = 0;
	projectTitleLabel->setSize({0, 240}, {1, 0});
	projectTitleLabel->setText(Editor::getInstance().getResources().getProject().getTitle());
	this->add(projectTitleLabel);

	auto buildButton = std::make_shared<edui::IconButton>();
	buildButton->iconId = ICON_HAMMER;
	buildButton->translationId = "Project.Build";
	buildButton->setTooltip();
	buildButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	this->add(buildButton);

	auto playButton = std::make_shared<edui::IconButton>();
	playButton->iconId = ICON_PLAYER_PLAY;
	playButton->translationId = "Project.Play";
	playButton->setTooltip();
	playButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});

	playButton->onClicked.connect([] { Editor::getInstance().getResources().getProject().spawnGame(); });

	this->add(playButton);
}

void ProjectTopBar::updateInfo() {
	projectTitleLabel->setText(Editor::getInstance().getResources().getProject().getTitle());
}
