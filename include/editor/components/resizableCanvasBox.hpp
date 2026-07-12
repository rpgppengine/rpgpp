#ifndef RPGPP_RESIZABLECANVASBOX_H
#define RPGPP_RESIZABLECANVASBOX_H
#include <string>

#include "components/resizeDirection.hpp"
#include "raylib.h"

class ResizableCanvasBox {
public:
	ResizableCanvasBox(std::string id, float x, float y, float width, float height, Color color,
					   bool isResizable = true);
	ResizableCanvasBox(std::string id, Rectangle rec, Color color, bool isResizable = true);
	void draw();

	// @returns true if the box was clicked, false otherwise
	bool leftMousePressed(Vector2 mousePos);
	void mouseMoved(Vector2 mousePos, int snapWidth = 1, int snapHeight = 1);
	bool mouseMovedTrigger(Vector2 mousePos, int snapWidth = 1, int snapHeight = 1);
	// @returns the new rectangle after the mouse is released
	Rectangle leftMouseReleased(Vector2 mousePos);

	Rectangle getRectangle();

	void updatePosition(float x, float y);
	void updateSize(float width, float height);
	void updateColor(Color color);
	void updateRec(Rectangle rec);

	void setResizeMargin(float newResizeMargin);

	bool focused = false;
	std::string id{};

	void setMinSize(float size) { minSize = std::max(size, 2.f); }
	float getMinSize() const { return minSize; }

private:
	float resizeMargin = 1.f;
	float minSize = 2.f;
	bool isResizable = true;

	float x, y, width, height;
	Color color = RED;

	bool isResizing = false;
	Vector2 startMousePos{};
	float prevX = 0, prevY = 0, prevWidth = 0, prevHeight = 0;
	int resizeDirection = NONE;
};

#endif
