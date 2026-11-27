#include "drawHelper.hpp"

#include "raylib.h"

void DrawRectangleLinesPro(Rectangle rect, Color color) {
	DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, color);
	DrawLine(rect.x, rect.y, rect.x, rect.y + rect.height, color);

	DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height, color);
	DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height, color);
}

bool RectangleEquals(const Rectangle &a, const Rectangle &b) {
	return (a.x == b.x) && (a.y == b.y) && (a.width == b.width) && (a.height == b.height);
}
