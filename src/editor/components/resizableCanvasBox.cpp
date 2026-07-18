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

int ResizableCanvasBox::inGrabber(Vector2 pos) {
	int direction = NONE;
	if (pos.x >= x && pos.x <= x + width && pos.y >= y && pos.y <= y + height) {
		if (isResizable) {
			if (pos.x >= x && pos.x <= x + resizeMargin) {
				direction |= LEFT;
			}

			if (pos.x >= x + width - resizeMargin && pos.x <= x + width) {
				direction |= RIGHT;
			}

			if (pos.y >= y && pos.y <= y + resizeMargin) {
				direction |= TOP;
			}

			if (pos.y >= y + height - resizeMargin && pos.y <= y + height) {
				direction |= BOTTOM;
			}
		}
	}
	return direction;
}

void ResizableCanvasBox::updateCursor(Vector2 absolutePos) {
	int hoveredGrabber = inGrabber(absolutePos);
	if (hoveredGrabber & (LEFT | RIGHT)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
		cursorModified = true;
	} else if (hoveredGrabber & (TOP | BOTTOM)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
		cursorModified = true;
	} else if (cursorModified) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		cursorModified = false;
	}
}

bool ResizableCanvasBox::leftMousePressed(Vector2 mousePos) {
	activeGrabber = NONE;

	if (mousePos.x >= x && mousePos.x <= x + width && mousePos.y >= y && mousePos.y <= y + height) {
		activeGrabber = inGrabber(mousePos);

		if (activeGrabber == NONE) {
			activeGrabber |= MOVE;
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
	updateCursor(mousePos);
	if (!isResizing || !focused) return false;

	bool movedIndicator = false;

	int dx = std::round((mousePos.x - startMousePos.x) / snapWidth) * snapWidth;
	int dy = std::round((mousePos.y - startMousePos.y) / snapHeight) * snapHeight;

	if ((activeGrabber & LEFT) && prevWidth - dx < minSize) dx = prevWidth - minSize;

	if ((activeGrabber & TOP) && prevHeight - dy < minSize) dy = prevHeight - minSize;

	if (activeGrabber & LEFT) {
		x = prevX + dx;
		width = prevWidth - dx;
		movedIndicator = true;
	}
	if (activeGrabber & RIGHT) {
		width = prevWidth + dx;
		movedIndicator = true;
	}
	if (activeGrabber & TOP) {
		y = prevY + dy;
		height = prevHeight - dy;
		movedIndicator = true;
	}
	if (activeGrabber & BOTTOM) {
		height = prevHeight + dy;
		movedIndicator = true;
	}
	if (activeGrabber & MOVE) {
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
		activeGrabber = NONE;
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
