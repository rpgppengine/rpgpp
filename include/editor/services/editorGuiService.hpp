#ifndef _RGPP_EDITOR_GUI_CONTAINER_H
#define _RGPP_EDITOR_GUI_CONTAINER_H

#include <map>
#include <memory>
#include <vector>

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "components/perfOverlay.hpp"
#include "raylib.h"
#include "screens/guiScreen.hpp"
#include "services/childWindowSubService.hpp"
#include "updatable.hpp"
constexpr auto RPGPP_EXECUTABLE_LOGO = "resources/app-icon.png";
constexpr int ACTION_BUTTON_SIZE = 16;

class EditorGuiService {
private:
	Font font;

	std::vector<std::weak_ptr<IUpdatable>> updatableWidgets;
	std::unique_ptr<ChildWindowSubService> childWindowService;
	PerformanceOverlay perfOverlay;

	int currentCursor = MOUSE_CURSOR_DEFAULT;
	bool leftMouseHeld = false;
	static const int MENUBAR_H = 32;

	std::weak_ptr<tgui::Group> screenContainer{};

public:
	bool isResettingUI = false;
	EditorGuiService();
	~EditorGuiService() = default;

	std::unique_ptr<tgui::Gui> gui;
	std::unique_ptr<UIScreen> currentScreen;
	std::deque<std::unique_ptr<UIScreen>> screenHistory;

	std::weak_ptr<tgui::MenuBar> menuBar{};
	std::map<std::string, std::string> translations = {};

	ChildWindowSubService *getChildWindowSubService();

	void init();
	void uiLoop();
	void initMenuBar();
	void addUpdate(std::shared_ptr<IUpdatable> widget);

	void setScreen(std::unique_ptr<UIScreen> newScreen, bool forceSwitch);
	void setScreen(UIScreen *newScreen, bool forceSwitch);

	void resetUi();
	tgui::Group::Ptr uiChangePreInit(UIScreen *setToScreen);
	void setResetUi() { this->isResettingUI = true; }

	void gotoPreviousScreen();
	void reloadUi();
	void alert(tgui::String title, tgui::String content);
	static void createLogoCenter(const tgui::GrowVerticalLayout::Ptr &layout);
	static void centerWidget(tgui::Widget::Ptr widget);

	Font getFont();
	void unload();
};

#endif
