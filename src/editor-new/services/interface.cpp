#include "services/interface.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/menuBar.hpp"
#include "widgets/mainMenuContainer.hpp"

using namespace editor;

Interface::Interface() {
	initEduiSettings();
	initMenuBar();
	initMainScreen();
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
	menuBar->addItem("file", {"one", "two"});
	menuBar->addItem("edit", {"one, two"});
	menuBar->onItemClicked.connect(
		[](const std::string &title, const std::string &option) { printf("%s %s \n", title.c_str(), option.c_str()); });
	menuBar->onItemClickedInt.connect([](size_t a, size_t b) { printf("%zu %zu \n", a, b); });

	gui.addMenuBar(menuBar);
}

void Interface::initMainScreen() {
	auto mainScreen = std::make_shared<edui::Container>();
	mainScreen->render->border = 0;
	mainScreen->setSize({1, 0}, {1, 0});

	auto baseCont = std::make_shared<MainMenuContainer>();
	baseCont->anchor = {0.5f, 0.5f};
	baseCont->setSize({0.7f, 0}, {0.7f, 0});
	mainScreen->add(baseCont);

	gui.add(mainScreen);
}
