#ifndef _RPGPP_NPATCHINFOWINDOW_H
#define _RPGPP_NPATCHINFOWINDOW_H

#include "childWindows/popupWindow.hpp"
#include "views/nPatchView.hpp"

class NPatchInfoWindow : public PopupWindow {
public:
	NPatchInfoWindow();

	NPatchView::Ptr view;
};

#endif