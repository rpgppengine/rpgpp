#ifndef _EDUI_TEXTBOX_H
#define _EDUI_TEXTBOX_H

#include "edui/label.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
struct TextBoxRender : public LabelRender {};

struct TextBox : public Widget {
	const float CursorWidth = 4.0f;
	const short DebounceFrames = 256;

	TextBox();
	void update();
	void draw();

	void setText(const std::string &text);

	void focused();
	void unfocused();
	void mouseEntered();
	void mouseLeft();
	void keyPressed(KeyboardKey key, bool held);
	void charEntered(char c);

	float scroll = 0;
	int cursorIndex = 0;
	Rectangle cursorRect = {0, 0, 0, 0};

	short debounce = 0;

protected:
	std::string text = "Hello!";
	void calcCursorRect();
};
}  // namespace edui

#endif
