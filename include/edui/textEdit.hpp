#ifndef _EDUI_TEXTEDIT_H
#define _EDUI_TEXTEDIT_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "edui/horizontalContainer.hpp"
#include "edui/horizontalScrollbar.hpp"
#include "edui/label.hpp"
#include "edui/verticalScrollbar.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct CursorPosition {
	uint16_t row = 0;
	uint16_t column = 0;
};

struct CharsRange {
	size_t start = 0;
	size_t end = 0;
};

struct PosRange {
	CursorPosition start;
	CursorPosition end;
};

struct TextEditRender : public LabelRender {};

struct TextEdit : public Widget {
	const float EDUI_TEXTEDIT_CURSOR_WIDTH = 4;
	const short EDUI_TEXTEDIT_DEBOUNCE = 5;
	const float EDUI_TEXTEDIT_SCROLLSCPEED = 8;

	std::shared_ptr<edui::VerticalScrollbar> scrollbar;

	TextEdit();
	~TextEdit();

	virtual void setText(const std::string &newText);

	void update() override;
	void draw() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void keyPressed(KeyboardKey key, KeyModifier mod, bool held) override;
	virtual void charEntered(int codepoint, std::string_view str) override;

	void mouseEntered() override;
	void mouseLeft() override;
	void mouseMoved(Vector2 mousePos, Vector2 relative) override;

	void scrolled(float mouseWheel) override;

	void unload();

protected:
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

	bool mouseHeld = false;
	CursorPosition selectStart;
	CursorPosition selectEnd;

	virtual void drawText(Vector2 textBegin);

	void drawCursor();
	void setCursorFromMouse();
	CursorPosition getPositionFromMouse();
	size_t calcCharPos(size_t row, size_t col);
	void setCursorRect();
	void reloadLines();

	bool hasSelection();
	PosRange normalizeSelection();
	void drawSelectionLine(PosRange range);
	void drawSelection();
	void resetSelection();
	void eraseSelection();
	void setOffset();

	void handleArrowKeys(KeyboardKey key);
	void handleDeletionKeys(KeyboardKey key);
	virtual void handleEnterTab(KeyboardKey key);
	void handleSelectionKeys(KeyboardKey key, KeyModifier mod);
};
}  // namespace edui

#endif
