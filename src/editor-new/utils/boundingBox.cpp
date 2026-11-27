#include "utils/boundingBox.hpp"

#include <cmath>

#include "drawHelper.hpp"
#include "edui/helper.hpp"
#include "raylib.h"
#include "raymath.h"

editor::BoundingBox::BoundingBox() { rect = Rectangle{0, 0, 50, 50}; }

void editor::BoundingBox::update() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isInRect()) {
		mouseOffset = Vector2Subtract(mousePos, {rect.x, rect.y});
	}

	updateBitset();
	updateMouseCursor();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		downResize = resize;
		previousRect = rect;
	}
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		downResize.reset();

		if (!RectangleEquals(previousRect, rect)) {
			mouseReleased.invoke(previousRect, rect);
		}
		previousRect = rect;
	}

	if (downResize.any() && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		auto oldRect = rect;
		float endX = oldRect.x + oldRect.width;
		float endY = oldRect.y + oldRect.height;

		if (downResize[RESIZE_ALL]) {
			Vector2 resultPos = Vector2Subtract(mousePos, mouseOffset);
			rect.x = resultPos.x;
			rect.y = resultPos.y;

			rect.x = snap * std::floor(rect.x / snap);
			rect.y = snap * std::floor(rect.y / snap);
			rect.width = snap * std::floor(rect.width / snap);
			rect.height = snap * std::floor(rect.height / snap);
		} else {
			if (downResize[RESIZE_LEFT]) {
				rect.x = mousePos.x;
				rect.x = snap * std::floor(rect.x / snap);

				rect.width = endX - rect.x;
				rect.width = snap * std::floor(rect.width / snap);
			}
			if (downResize[RESIZE_TOP]) {
				rect.y = mousePos.y;
				rect.y = snap * std::floor(rect.y / snap);

				rect.height = endY - rect.y;
				rect.height = snap * std::floor(rect.height / snap);
			}

			if (downResize[RESIZE_RIGHT]) {
				rect.width = mousePos.x - rect.x;
				rect.width = snap * std::floor(rect.width / snap);
			}
			if (downResize[RESIZE_BOTTOM]) {
				rect.height = mousePos.y - rect.y;
				rect.height = snap * std::floor(rect.height / snap);
			}
		}

		if (!RectangleEquals(oldRect, rect)) {
			rectChanged.invoke(oldRect, rect);
		}
	}
}

void editor::BoundingBox::updateBitset() {
	if (isInRect()) {
		resize.reset();

		auto padded = paddingRect(rect, padding);
		if (CheckCollisionPointRec(mousePos, padded)) {
			resize.reset();
			resize.set(RESIZE_ALL);
		} else {
			if (mousePos.x < padded.x) {
				resize.set(RESIZE_LEFT);
			}
			if (mousePos.x > (padded.x + padded.width)) {
				resize.set(RESIZE_RIGHT);
			}

			if (mousePos.y < padded.y) {
				resize.set(RESIZE_TOP);
			}
			if (mousePos.y > (padded.y + padded.height)) {
				resize.set(RESIZE_BOTTOM);
			}
		}
	} else {
		resize.reset();
	}
}

void editor::BoundingBox::updateMouseCursor() {
	if (resize.any()) {
		if (resize[RESIZE_ALL]) {
			setMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
		}
		if (resize[RESIZE_LEFT] || resize[RESIZE_RIGHT]) {
			setMouseCursor(MOUSE_CURSOR_RESIZE_EW);
		}
		if (resize[RESIZE_TOP] || resize[RESIZE_BOTTOM]) {
			setMouseCursor(MOUSE_CURSOR_RESIZE_NS);
		}
		if ((resize[RESIZE_LEFT] && resize[RESIZE_TOP]) || (resize[RESIZE_RIGHT] && resize[RESIZE_BOTTOM])) {
			setMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
		}
		if ((resize[RESIZE_LEFT] && resize[RESIZE_BOTTOM]) || (resize[RESIZE_RIGHT] && resize[RESIZE_TOP])) {
			setMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
		}
	} else {
		setMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
}

void editor::BoundingBox::draw() {
	DrawRectangleRec(rect, Fade(MAROON, 0.2f));
	DrawRectangleLinesEx(rect, padding, Fade(MAROON, 0.8f));
}

void editor::BoundingBox::setMousePos(Vector2 mousePos) { this->mousePos = mousePos; }

Rectangle editor::BoundingBox::getRectangle() { return rect; }

void editor::BoundingBox::setRectangle(const Rectangle &rect) {
	this->rect = rect;
	previousRect = rect;
}

void editor::BoundingBox::setSnap(float snap) { this->snap = snap; }

bool editor::BoundingBox::isInRect() { return CheckCollisionPointRec(mousePos, rect); }

void editor::BoundingBox::setMouseCursor(MouseCursor cursor) {
	if (cursor != this->lastCursor) {
		this->lastCursor = cursor;
		SetMouseCursor(cursor);
	}
}
