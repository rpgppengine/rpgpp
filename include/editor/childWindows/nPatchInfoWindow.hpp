#ifndef _RPGPP_NPATCHINFOWINDOW_H
#define _RPGPP_NPATCHINFOWINDOW_H

#include "TGUI/Widgets/Panel.hpp"
#include "childWindows/popupWindow.hpp"
#include "raylib.h"
#include "views/nPatchView.hpp"
#include "widgets/propertyFields/intField.hpp"

class NPatchInfoWindow : public PopupWindow {
private:
	const float FIELD_WIDTH = 130.0f;

public:
	NPatchInfoWindow();

	tgui::Panel::Ptr topPanel;
	IntField::Ptr top;
	IntField::Ptr left;
	IntField::Ptr bottom;
	IntField::Ptr right;

	NPatchView::Ptr view;

	void init(NPatchInfo *info, Texture texture);
};

#endif