#ifndef _RPGPP_ELEMENTRENAMEWINDOW_H
#define _RPGPP_ELEMENTRENAMEWINDOW_H

#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "childWindows/popupWindow.hpp"
#include "interfaceView.hpp"

class ElementRenameWindow : public PopupWindow {
private:
	tgui::EditBox::Ptr input;
public:
	InterfaceView *view = nullptr;
	tgui::TreeView *tree = nullptr;
	ElementIndex elementIndex = MAX_ELEMENTS;
	ElementRenameWindow();
	void init();

	void onRenameElement(ElementIndex index, const std::string& newName);
};

#endif
