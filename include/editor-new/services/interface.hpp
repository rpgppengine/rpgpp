#ifndef _EDITOR_SERVICES_INTERFACE_H
#define _EDITOR_SERVICES_INTERFACE_H

#include <memory>

#include "edui/container.hpp"
#include "edui/gui.hpp"

namespace editor {
const int INTERFACE_SPACING = 4;

class Interface {
public:
	Interface();

	void update();
	void draw();
	void unload();

	edui::Gui &getEdui();
	void showProjectScreen();

private:
	edui::Gui gui;
	std::shared_ptr<edui::Container> mainContainer;

	void initEduiSettings();
	void initMenuBar();
	void initMainScreen();
};
}  // namespace editor

#endif
