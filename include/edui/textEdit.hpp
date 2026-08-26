#ifndef _EDUI_TEXTEDIT_H
#define _EDUI_TEXTEDIT_H

#include <cstddef>
#include <cstdint>
#include <string>

#include "edui/label.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct CursorPosition {
	uint16_t row = 0;
	uint16_t column = 0;
};

struct TextEditRender : public LabelRender {};

struct TextEdit : public Widget {
	const float EDUI_TEXTEDIT_CURSOR_WIDTH = 4;
	const short EDUI_TEXTEDIT_DEBOUNCE = 5;
	const float EDUI_TEXTEDIT_SCROLLSCPEED = 8;

	TextEdit();
	~TextEdit();

	void setText(const std::string &newText);

	void update() override;
	void draw() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void keyPressed(KeyboardKey key, KeyModifier mod, bool held) override;
	void charEntered(int codepoint, std::string_view str) override;

	void mouseEntered() override;
	void mouseLeft() override;

	void scrolled(float mouseWheel) override;

	void unload();

private:
	short debounce = 0;

	Rectangle scissorContentRect;
	float scissorX = 0;
	float scissorY = 0;

	float scrollMax = 0.0f;
	bool overflownY = false;

	std::string text = "";
	int rowCount = 0;
	char **lines = nullptr;

	CursorPosition cursorPos;
	Rectangle cursorRect = {0, 0, EDUI_TEXTEDIT_CURSOR_WIDTH, 16};
	size_t charPos = 0;

	void drawCursor();
	void setCursorFromMouse();
	size_t calcCharPos(size_t row, size_t col);
	void setCursorRect();
	void reloadLines();

	void setOffset();

	void handleArrowKeys(KeyboardKey key);
	void handleDeletionKeys(KeyboardKey key);
	void handleEnterTab(KeyboardKey key);
};
}  // namespace edui

#endif
