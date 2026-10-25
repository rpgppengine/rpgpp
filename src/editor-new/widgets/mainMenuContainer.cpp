#include "widgets/mainMenuContainer.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/imageRect.hpp"
#include "edui/label.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"
#include "widgets/recentProjectsList.hpp"

using namespace editor;

MainMenuContainer::MainMenuContainer() {
	initTopContainer();
	initBottomContainer();
}

void MainMenuContainer::initTopContainer() {
	auto topCont = std::make_shared<edui::Container>();
	topCont->render->border = 0;
	topCont->setSize({1, 0}, {0.4f, 0});
	add(topCont);

	Texture logo = Editor::getInstance().getResources().getTexture("logo-ups.png");
	auto logoImg = std::make_shared<edui::ImageRect>();
	logoImg->render->border = 0;
	logoImg->setTexture(logo);
	logoImg->resizeToTexture();
	logoImg->anchor = {0.5f, 0.5f};
	topCont->add(logoImg);
}

void MainMenuContainer::initBottomContainer() {
	auto bottomCont = std::make_shared<edui::Container>();
	bottomCont->render->border = 0;
	bottomCont->setSize({1, 0}, {0.6f, 0});
	bottomCont->setPosition({0, 0}, {0.4f, 0});
	add(bottomCont);

	auto sideOptions = std::make_shared<edui::VerticalContainer>();
	sideOptions->render->border = 0;
	sideOptions->render->padding = INTERFACE_SPACING;
	sideOptions->render->as<edui::VerticalContainerRender>().space = INTERFACE_SPACING;
	sideOptions->setPosition({0, 0}, {0, edui::EDUI_SECONDARY_HEIGHT - INTERFACE_SPACING});
	sideOptions->setSize({0.4f, 0}, {1, -edui::EDUI_SECONDARY_HEIGHT});

	auto newProjectButton = std::make_shared<edui::Button>();
	newProjectButton->render->as<edui::LabelRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	newProjectButton->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	newProjectButton->setText("New Project");
	newProjectButton->translationId = "MainMenu.Options.New";
	sideOptions->add(newProjectButton);

	auto openProjectButton = std::make_shared<edui::Button>();
	openProjectButton->render->as<edui::LabelRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	openProjectButton->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	openProjectButton->setText("Open Project");
	openProjectButton->translationId = "MainMenu.Options.Open";
	sideOptions->add(openProjectButton);

	bottomCont->add(sideOptions);

	auto label = std::make_shared<edui::Label>();
	label->render->border = 0;
	label->setPosition({0.4f, 0}, {0, 0});
	label->setSize({0.6f, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	label->setText("Recent Projects");
	label->translationId = "MainMenu.RecentProjects";
	bottomCont->add(label);

	auto list = std::make_shared<RecentProjectsList>();
	list->setPosition({0.4f, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	list->setSize({0.6f, -INTERFACE_SPACING}, {1, -(INTERFACE_SPACING + edui::EDUI_SECONDARY_HEIGHT)});

	bottomCont->add(list);
}
