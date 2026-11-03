#ifndef _EDITOR_SERVICES_INTERFACE_H
#define _EDITOR_SERVICES_INTERFACE_H

#include <memory>
#include <string>

#include "edui/childWindow.hpp"
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

	void showTopWindow(std::shared_ptr<edui::ChildWindow> widget);
	void showErrorMessage(const std::string &message);

private:
	edui::Gui gui;
	std::shared_ptr<edui::Container> mainContainer;
	bool hasTopWindow = false;

	void initEduiSettings();
	void initMenuBar();
	void initMainScreen();
};
}  // namespace editor

#endif
