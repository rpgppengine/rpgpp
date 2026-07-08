#ifndef _RPGPP_UI_ELEMENTS_VALUESLIDER_H
#define _RPGPP_UI_ELEMENTS_VALUESLIDER_H

#include "uiElement.hpp"

struct ValueSlider : public UIElement {
	ValueSlider();
	void init();
	void update();
	void draw(Rectangle rect);
	void onNotify(Event event);
	void config();

	Color currentColor = RAYWHITE;
};

#endif
