#include "services/interface.hpp"

#include <cstddef>
#include <memory>

#include "editor.hpp"
#include "edui/gui.hpp"
#include "edui/menuBar.hpp"
#include "edui/messageBox.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "utils/editorConsts.hpp"
#include "widgets/screens/mainMenuScreen.hpp"
#include "widgets/screens/projectScreen.hpp"
#include "widgets/viewerPanel.hpp"

using namespace editor;

Interface::Interface() {
	initEduiSettings();
	initMenuBar();
	initMainScreen();

	Editor::getInstance().getResources().getProject().projectOpened.connect([this] { showProjectScreen(); });

	gui.addKeyEvent([this](KeyboardKey key, edui::KeyModifier mod) { handleKeybinds(key, mod); });
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

	gui.loadIconsFile("resources/iconset.rgi");
}

void Interface::initMenuBar() {
	auto menuBar = std::make_shared<edui::MenuBar>();
	menuBar->translationId = "Menu";
	menuBar->addItem("file", {"open", "new", "savefile"});
	menuBar->addItem("edit", {"undo", "redo"});
	menuBar->onItemClicked.connect(
		[](const std::string &title, const std::string &option) { printf("%s %s \n", title.c_str(), option.c_str()); });
	menuBar->onItemClickedInt.connect([this](size_t a, size_t b) {
		printf("%zu %zu \n", a, b);
		if (a == 0) {
			handleFileMenu(b);
		}
		if (a == 1) {
			handleEditMenu(b);
		}
	});

	gui.addMenuBar(menuBar);
}

void Interface::handleFileMenu(size_t option) {
	switch (option) {
		case 0: {
			Editor::getInstance().getResources().onOpenProject();
		} break;
		case 1: {
			Editor::getInstance().getResources().onNewProject();
		} break;
		case 2: {
			getViewerPanel()->getCurrent()->saveFile();
		} break;
	}
}

void Interface::handleEditMenu(size_t option) {
	switch (option) {
		case 0: {
			getViewerPanel()->getCurrent()->undo();
		} break;
		case 1: {
			getViewerPanel()->getCurrent()->redo();
		} break;
	}
}

void Interface::handleKeybinds(KeyboardKey key, edui::KeyModifier mod) {
	if (mod.ctrl) {
		if (key == KEY_S) {
			getViewerPanel()->getCurrent()->saveFile();
		}
		if (key == KEY_Z) {
			getViewerPanel()->getCurrent()->undo();
		}
		if (key == KEY_X) {
			getViewerPanel()->getCurrent()->redo();
		}
	}
}

void Interface::initMainScreen() {
	auto mainScreen = std::make_shared<MainMenuScreen>();
	mainScreen->render->border = 0;
	mainScreen->setSize({1, 0}, {1, 0});
	mainContainer = mainScreen;

	gui.add(mainScreen);
}

void Interface::showTopWindow(std::shared_ptr<edui::ChildWindow> widget) {
	if (hasTopWindow) return;
	this->hasTopWindow = true;
	gui.add(widget, edui::EDUI_MAX_LAYERS - 1);
	widget->onDeleted.connect([this] { this->hasTopWindow = false; });
}

void Interface::showErrorMessage(const std::string &message) {
	auto messageBox = std::make_shared<edui::MessageBox>();
	messageBox->setSize({0, EDITOR_CHILD_WINDOW_WIDTH}, {0, EDITOR_CHILD_WINDOW_HEIGHT});
	messageBox->centerLayout();
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
	std::string windowTitle = "Editor - " + Editor::getInstance().getResources().getProject().getTitle();
	SetWindowTitle(windowTitle.c_str());

	this->isInProjectScreen = true;
}

std::shared_ptr<ViewerPanel> Interface::getViewerPanel() {
	if (!isInProjectScreen) return nullptr;

	auto &projectScreen = mainContainer->as<ProjectScreen>();
	return projectScreen.getViewer();
}
