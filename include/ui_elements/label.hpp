#ifndef _RPGPP_UI_ELEMENTS_LABEL_H
#define _RPGPP_UI_ELEMENTS_LABEL_H

#include "ui_elements/uiElement.hpp"

struct Label : public UIElement {
	Label();
	void init();
	void update();
	void draw(Rectangle rect);

	void config();
	void loadFont(const std::string &path);
};

#endif
