#include "widgets/screens/mainMenuScreen.hpp"

#include "edui/container.hpp"
#include "widgets/mainMenuPanel.hpp"

using namespace editor;

MainMenuScreen::MainMenuScreen() : edui::Container() {
	auto baseCont = std::make_shared<MainMenuPanel>();
	baseCont->anchor = {0.5f, 0.5f};
	baseCont->setSize({0.7f, 0}, {0.7f, 0});
	add(baseCont);
}
