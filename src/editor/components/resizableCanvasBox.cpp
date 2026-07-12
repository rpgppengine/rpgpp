#include "components/resizableCanvasBox.hpp"

#include <cmath>

#include "raylib.h"

bool bounded(float value, float min, float max) { return value >= min && value <= max; }

ResizableCanvasBox::ResizableCanvasBox(std::string id, float x, float y, float width, float height, Color color,
									   bool isResizable)
	: id(id), x(x), y(y), width(width), height(height), color(color), isResizable(isResizable) {}

ResizableCanvasBox::ResizableCanvasBox(std::string id, Rectangle rec, Color color, bool isResizable)
	: id(id), x(rec.x), y(rec.y), width(rec.width), height(rec.height), color(color), isResizable(isResizable) {}

void ResizableCanvasBox::updateSize(float width, float height) {
	this->width = width;
	this->height = height;
}

void ResizableCanvasBox::updatePosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void ResizableCanvasBox::updateColor(Color color) { this->color = color; }

void ResizableCanvasBox::updateRec(Rectangle rec) {
	this->x = rec.x;
	this->y = rec.y;
	this->width = rec.width;
	this->height = rec.height;
}

Rectangle ResizableCanvasBox::getRectangle() { return {this->x, this->y, this->width, this->height}; }

void ResizableCanvasBox::setResizeMargin(float newResizeMargin) { this->resizeMargin = newResizeMargin; }

bool ResizableCanvasBox::leftMousePressed(Vector2 mousePos) {
	resizeDirection = NONE;

	if (mousePos.x >= x && mousePos.x <= x + width && mousePos.y >= y && mousePos.y <= y + height) {
		if (isResizable) {
			if (mousePos.x >= x && mousePos.x <= x + resizeMargin) {
				resizeDirection |= LEFT;
			}

			if (mousePos.x >= x + width - resizeMargin && mousePos.x <= x + width) {
				resizeDirection |= RIGHT;
			}

			if (mousePos.y >= y && mousePos.y <= y + resizeMargin) {
				resizeDirection |= TOP;
			}

			if (mousePos.y >= y + height - resizeMargin && mousePos.y <= y + height) {
				resizeDirection |= BOTTOM;
			}
		}

		if (resizeDirection == NONE) {
			resizeDirection |= MOVE;
		}

		isResizing = true;
		startMousePos = mousePos;
		prevX = x;
		prevY = y;
		prevWidth = width;
		prevHeight = height;

		return true;
	}
	return false;
}

bool ResizableCanvasBox::mouseMovedTrigger(Vector2 mousePos, int snapWidth, int snapHeight) {
	if (!isResizing || !focused) return false;

	bool movedIndicator = false;

	int dx = std::round((mousePos.x - startMousePos.x) / snapWidth) * snapWidth;
	int dy = std::round((mousePos.y - startMousePos.y) / snapHeight) * snapHeight;

	if ((resizeDirection & LEFT) && prevWidth - dx < minSize) dx = prevWidth - minSize;

	if ((resizeDirection & TOP) && prevHeight - dy < minSize) dy = prevHeight - minSize;

	if (resizeDirection & LEFT) {
		x = prevX + dx;
		width = prevWidth - dx;
		movedIndicator = true;
	}
	if (resizeDirection & RIGHT) {
		width = prevWidth + dx;
		movedIndicator = true;
	}
	if (resizeDirection & TOP) {
		y = prevY + dy;
		height = prevHeight - dy;
		movedIndicator = true;
	}
	if (resizeDirection & BOTTOM) {
		height = prevHeight + dy;
		movedIndicator = true;
	}
	if (resizeDirection & MOVE) {
		x = prevX + dx;
		y = prevY + dy;
		movedIndicator = true;
	}

	width = std::max(minSize, width);
	height = std::max(minSize, height);

	return movedIndicator;
}

void ResizableCanvasBox::mouseMoved(Vector2 mousePos, int snapWidth, int snapHeight) {
	this->mouseMovedTrigger(mousePos, snapWidth, snapHeight);
}

Rectangle ResizableCanvasBox::leftMouseReleased(Vector2 mousePos) {
	if (isResizing) {
		isResizing = false;
		resizeDirection = NONE;
	}
	return Rectangle{x, y, width, height};
}

void ResizableCanvasBox::draw() {
	float opacity = 0.2;
	if (focused) {
		opacity = 0.5;
	}
	DrawRectangleLinesEx(Rectangle{x, y, width, height}, resizeMargin, Fade(BLACK, opacity));
	DrawRectangleRec(Rectangle{x, y, width, height}, Fade(color, opacity));
	// Implement draw logic here
}
