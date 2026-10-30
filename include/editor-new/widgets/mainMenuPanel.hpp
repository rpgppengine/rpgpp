#ifndef _EDITOR_WIDGETS_MAINMENUSCREEN_H
#define _EDITOR_WIDGETS_MAINMENUSCREEN_H

#include "edui/container.hpp"

namespace editor {
struct MainMenuPanel : public edui::Container {
	MainMenuPanel();

private:
	void initTopContainer();
	void initBottomContainer();
	void onOpenProjectButton();
};
}  // namespace editor

#endif
