#ifndef _RPGPP_UI_ELEMENTS_NINEPATCHIMAGERECT_H
#define _RPGPP_UI_ELEMENTS_NINEPATCHIMAGERECT_H

#include "ui_elements/uiElement.hpp"

struct NinePatchImageRect : public UIElement {
	NinePatchImageRect();
	void init();
	void update();
	void draw(Rectangle rect);

	void config();
	void scaleImage(int scale);
	void loadImage(const std::string& path);
};

#endif
