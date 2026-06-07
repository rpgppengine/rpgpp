#include "drawHelper.hpp"

#include "raylib.h"

void DrawRectangleLinesPro(Rectangle rect, Color color) {
	DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, color);
	DrawLine(rect.x, rect.y, rect.x, rect.y + rect.height, color);

	DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height, color);
	DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height, color);
}