#ifndef _EDUI_TEXTBOX_H
#define _EDUI_TEXTBOX_H

#include <string_view>
#include "edui/label.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
struct SelectionResult {
	int start = 0;
	int end = 0;
	bool hasError = false;
};

struct TextBoxRender : public LabelRender {};

struct TextBox : public Widget {
	const float CursorWidth = 4.0f;
	const short DebounceFrames = 5;

	TextBox();
	void update();
	void draw();

	void setText(const std::string &text);

	void focused();
	void unfocused();
	void mouseEntered();
	void mouseLeft();
	void keyPressed(KeyboardKey key, KeyModifier mod, bool held);
	void charEntered(int codepoint, std::string_view str);
	void leftMouseClicked();
	void leftMouseReleased();

	float scroll = 0;
	int cursorIndex = 0;
	Rectangle cursorRect = {0, 0, 0, 0};

	short debounce = 0;

	bool mouseBtnHeld = false;
	int selectStart = 0;
	int selectEnd = 0;

protected:
	std::string text = "Hello!";
	void calcCursorRect();
	int calcCursor();
	SelectionResult calcSelection();
	SelectionResult getSelectionInfo();
	void drawSelection();
};
}  // namespace edui

#endif
