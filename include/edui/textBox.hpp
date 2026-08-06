#ifndef _EDUI_TEXTBOX_H
#define _EDUI_TEXTBOX_H

#include <string>
#include <string_view>

#include "edui/label.hpp"
#include "edui/widget.hpp"
#include "edui/valuewidget.hpp"
#include "raylib.h"

namespace edui {
struct SelectionResult {
	int start = 0;
	int end = 0;
	bool hasError = false;
};

struct TextBoxRender : public LabelRender {};

struct TextBox : public Widget, public ValueWidgetT<std::string> {
	const float CursorWidth = 4.0f;
	const short DebounceFrames = 5;

	void setValue(const std::string &text) override;
	std::string getValue() override;

	TextBox();
	void update() override;
	void draw() override;

	void setText(const std::string &text);

	void focused() override;
	void unfocused() override;
	void mouseEntered() override;
	void mouseLeft() override;
	void keyPressed(KeyboardKey key, KeyModifier mod, bool held) override;
	void charEntered(int codepoint, std::string_view str) override;
	void leftMouseClicked() override;
	void leftMouseReleased() override;

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
