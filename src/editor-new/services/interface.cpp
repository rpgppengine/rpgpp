#include "services/interface.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/gui.hpp"
#include "edui/menuBar.hpp"
#include "edui/messageBox.hpp"
#include "raylib.h"
#include "widgets/screens/mainMenuScreen.hpp"
#include "widgets/screens/projectScreen.hpp"

using namespace editor;

Interface::Interface() {
	initEduiSettings();
	initMenuBar();
	initMainScreen();

	Editor::getInstance().getResources().getProject().projectOpened.connect([this] { showProjectScreen(); });
}

void Interface::update() { gui.update(); }

void Interface::draw() { gui.draw(); }

void Interface::unload() { gui.unload(); }

edui::Gui &Interface::getEdui() { return gui; }

void Interface::initEduiSettings() {
	gui.loadThemesDir("resources/themes");
	gui.loadTheme("RPG++ Light");

	gui.loadTranslationNames("resources/langs.ini");
	gui.loadTranslation("resources/en.ini");

	gui.setFont("resources/TerminusTTF-4.49.3.ttf", 96, 18, 0);
}

void Interface::initMenuBar() {
	auto menuBar = std::make_shared<edui::MenuBar>();
	menuBar->translationId = "Menu";
	menuBar->addItem("file", {"open", "new"});
	menuBar->addItem("edit", {"one, two"});
	menuBar->onItemClicked.connect(
		[](const std::string &title, const std::string &option) { printf("%s %s \n", title.c_str(), option.c_str()); });
	menuBar->onItemClickedInt.connect([](size_t a, size_t b) {
		printf("%zu %zu \n", a, b);
		if (a == 0) {
			switch (b) {
				case 0: {
					Editor::getInstance().getResources().onOpenProject();
				} break;
				case 1: {
					Editor::getInstance().getResources().onNewProject();
				} break;
			}
		}
	});

	gui.addMenuBar(menuBar);
}

void Interface::initMainScreen() {
	auto mainScreen = std::make_shared<MainMenuScreen>();
	mainScreen->render->border = 0;
	mainScreen->setSize({1, 0}, {1, 0});
	mainContainer = mainScreen;

	gui.add(mainScreen);
}

void Interface::showErrorMessage(const std::string &message) {
	auto messageBox = std::make_shared<edui::MessageBox>();
	messageBox->setText(message);
	gui.add(messageBox, edui::EDUI_MAX_LAYERS - 1);
}

void Interface::showProjectScreen() {
	mainContainer->markDelete();

	auto projectScreen = std::make_shared<ProjectScreen>();
	projectScreen->render->border = 0;
	projectScreen->setSize({1, 0}, {1, 0});
	gui.add(projectScreen);
	mainContainer = projectScreen;

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	MaximizeWindow();
}
