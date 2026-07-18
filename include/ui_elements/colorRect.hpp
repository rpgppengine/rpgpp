#ifndef _RPGPP_UI_ELEMENTS_COLORRECT_H
#define _RPGPP_UI_ELEMENTS_COLORRECT_H

#include "ui_elements/uiElement.hpp"

struct ColorRect : public UIElement {
	ColorRect();
	void init();
	void update();
	void draw(Rectangle rect);
};

#endif
