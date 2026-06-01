#ifndef _RPGPP_ELEMENTINITWINDOW_H
#define _RPGPP_ELEMENTINITWINDOW_H

#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/popupWindow.hpp"
#include "interfaceView.hpp"

class ElementInitWindow : public PopupWindow {
private:
	tgui::EditBox::Ptr input;
	tgui::TreeView::Ptr list;

public:
	InterfaceView *view = nullptr;
	tgui::TreeView *tree = nullptr;
	ElementInitWindow();
	void init();
};

#endif