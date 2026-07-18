#ifndef _RPGPP_UI_ELEMENTS_IMAGERECT_H
#define _RPGPP_UI_ELEMENTS_IMAGERECT_H

#include "ui_elements/uiElement.hpp"

struct ImageRect : public UIElement {
	ImageRect();
	void init();
	void update();
	void draw(Rectangle rect);

	void config();
	void scaleImage(int scale);
	void loadImage(const std::string &path);
};

#endif
