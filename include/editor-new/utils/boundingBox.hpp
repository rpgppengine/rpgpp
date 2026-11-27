#ifndef _EDITOR_UTILS_BOUNDINGBOX_H
#define _EDITOR_UTILS_BOUNDINGBOX_H

#include <bitset>

#include "edui/signal.hpp"
#include "raylib.h"
namespace editor {
enum BoxResizeDirection { RESIZE_LEFT = 1, RESIZE_RIGHT, RESIZE_TOP, RESIZE_BOTTOM, RESIZE_ALL };

class BoundingBox {
public:
	edui::SignalT2<Rectangle, Rectangle> rectChanged;
	edui::SignalT2<Rectangle, Rectangle> mouseReleased;

	BoundingBox();

	void update();
	void draw();

	void setMousePos(Vector2 mousePos);

	void setRectangle(const Rectangle &rect);
	Rectangle getRectangle();

	void setSnap(float snap);

private:
	Vector2 mousePos = {0, 0};

	Rectangle rect;
	Rectangle previousRect;
	float padding = 2;

	MouseCursor lastCursor = MOUSE_CURSOR_DEFAULT;
	std::bitset<8> resize = 0;
	std::bitset<8> downResize = 0;
	Vector2 mouseOffset = {0, 0};
	float snap = 1;

	bool isInRect();
	void setMouseCursor(MouseCursor cursor);
	void updateBitset();
	void updateMouseCursor();
};
}  // namespace editor

#endif
