#ifndef _EDITOR_UTILS_GRIDDRAW_H
#define _EDITOR_UTILS_GRIDDRAW_H

#include "basicTypes.hpp"
#include "raylib.h"
namespace editor {
struct GridDraw {
	int tileWidth = 16;
	int tileHeight = 16;
	int rows = 5;
	int columns = 5;
	Color color = GRAY;

	void draw();
	IVector getTileAtPosition(Vector2 position);
	Rectangle getTileRect(IVector pos);
};
}  // namespace editor

#endif
