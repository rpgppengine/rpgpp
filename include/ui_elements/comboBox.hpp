#ifndef _RPGPP_UI_ELEMENTS_COMBOBOX_H
#define _RPGPP_UI_ELEMENTS_COMBOBOX_H

#include "uiElement.hpp"

struct ComboBox : public UIElement {
	ComboBox();
	void init();
	void update();
	void draw(Rectangle rect);
	void onNotify(Event event);
	void config();

	void loadFont(const std::string &path);

	Color shownTextColor = RAYWHITE;
};

#endif
