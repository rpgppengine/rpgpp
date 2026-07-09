#ifndef _RPGPP_UI_ELEMENTS_TEXTBOX_H
#define _RPGPP_UI_ELEMENTS_TEXTBOX_H

#include "uiElement.hpp"

struct TextBox : public UIElement {
	TextBox();
	void init();
	void update();
	void draw(Rectangle rect);
	void onNotify(Event event);
	void config();

	void loadFont(const std::string &path);

	Color shownTextColor = RAYWHITE;
	int cursorPosition = 0;

	int debounce = 0;
};

#endif
