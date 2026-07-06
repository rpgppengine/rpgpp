#ifndef _RPGPP_UI_ELEMENTS_BUTTON_H
#define _RPGPP_UI_ELEMENTS_BUTTON_H

#include "ui_elements/uiElement.hpp"

struct Button : public UIElement {
	Button();
	void init();
	void update();
	void draw(Rectangle rect);

	Color shownTextColor = BLACK;

	void config();
	void loadFont(const std::string& path);
};

#endif
