#ifndef _EDITOR_WIDGETS_MAINMENUCONTAINER_H
#define _EDITOR_WIDGETS_MAINMENUCONTAINER_H

#include "edui/container.hpp"

namespace editor {
struct MainMenuContainer : public edui::Container {
	MainMenuContainer();

private:
	void initTopContainer();
	void initBottomContainer();
};
}  // namespace editor

#endif
