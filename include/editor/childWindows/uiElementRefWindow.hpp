#ifndef _RPGPP_UIELEMENTREFWINDOW_H
#define _RPGPP_UIELEMENTREFWINDOW_H

#include <cstdint>

#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/popupWindow.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "widgets/propertyFields/uiElementRefField.hpp"

class UIElementRefWindow : public PopupWindow {
private:
	tgui::ScrollablePanel::Ptr panel;
	tgui::GrowVerticalLayout::Ptr layout;
	void addButton(const std::string &title, uint8_t entityId);

public:
	InterfaceView *view = nullptr;
	UIElementRef *ref = nullptr;
	UIElementRefField *field = nullptr;
	UIElementRefWindow();
	void init();
};

#endif