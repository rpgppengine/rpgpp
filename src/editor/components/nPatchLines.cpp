#include "components/nPatchLines.hpp"

#include <cmath>
#include <cstdio>

#include "components/resizeDirection.hpp"
#include "raylib.h"
#include "raymath.h"

NPatchLines::NPatchLines() {}

void NPatchLines::draw() {
	if (info == nullptr) return;
	if (!IsTextureValid(texture)) return;

	calcCorners();

	// top and bottom
	DrawLine(0, info->top * scale, texture.width, info->top * scale, RED);
	DrawLine(0, texture.height - (info->bottom * scale), texture.width, texture.height - (info->bottom * scale), RED);

	// left and right
	DrawLine(info->left * scale, 0, info->left * scale, texture.height, RED);
	DrawLine(texture.width - (info->right * scale), 0, texture.width - (info->right * scale), texture.height, RED);

	float handleRadius = HANDLE_RADIUS * scale;

	// draw handles
	DrawCircleV(topLeft, handleRadius, MAROON);
	DrawCircleV(topRight, handleRadius, MAROON);

	DrawCircleV(bottomLeft, handleRadius, MAROON);
	DrawCircleV(bottomRight, handleRadius, MAROON);

	const Color hoverColor = {90, 33, 55, 255};

	// draw hovered
	switch (corner) {
		case CORNER_NONE:
			break;
		case CORNER_TOPLEFT:
			DrawCircleV(topLeft, handleRadius, hoverColor);
			break;
		case CORNER_TOPRIGHT:
			DrawCircleV(topRight, handleRadius, hoverColor);
			break;
		case CORNER_BOTTOMLEFT:
			DrawCircleV(bottomLeft, handleRadius, hoverColor);
			break;
		case CORNER_BOTTOMRIGHT:
			DrawCircleV(bottomRight, handleRadius, hoverColor);
			break;
	}
}

bool NPatchLines::isInRect(Vector2 mousePos) {
	if (mousePos.x >= 0 && mousePos.x <= texture.width && mousePos.y >= 0 && mousePos.y <= texture.height) {
		return true;
	}
	return false;
}

void NPatchLines::calcCorners() {
	topLeft = {static_cast<float>(info->left * scale), static_cast<float>(info->top * scale)};
	topRight = {texture.width - static_cast<float>(info->right * scale), static_cast<float>(info->top * scale)};
	bottomLeft = {static_cast<float>(info->left * scale), texture.height - static_cast<float>(info->bottom * scale)};
	bottomRight = {texture.width - static_cast<float>(info->right * scale),
				   texture.height - static_cast<float>(info->bottom * scale)};
}

void NPatchLines::detectCorner(Vector2 mousePos) {
	corner = CORNER_NONE;
	float handleRadius = HANDLE_RADIUS * scale;

	if (mousePos.x >= (topLeft.x - handleRadius) && mousePos.x <= (topLeft.x + handleRadius) &&
		mousePos.y >= (topLeft.y - handleRadius) && mousePos.y <= (topLeft.y + handleRadius)) {
		corner = CORNER_TOPLEFT;
	} else if (mousePos.x >= (topRight.x - handleRadius) && mousePos.x <= (topRight.x + handleRadius) &&
			   mousePos.y >= (topRight.y - handleRadius) && mousePos.y <= (topRight.y + handleRadius)) {
		corner = CORNER_TOPRIGHT;
	} else if (mousePos.x >= (bottomLeft.x - handleRadius) && mousePos.x <= (bottomLeft.x + handleRadius) &&
			   mousePos.y >= (bottomLeft.y - handleRadius) && mousePos.y <= (bottomLeft.y + handleRadius)) {
		corner = CORNER_BOTTOMLEFT;
	} else if (mousePos.x >= (bottomRight.x - handleRadius) && mousePos.x <= (bottomRight.x + handleRadius) &&
			   mousePos.y >= (bottomRight.y - handleRadius) && mousePos.y <= (bottomRight.y + handleRadius)) {
		corner = CORNER_BOTTOMRIGHT;
	}
}

bool NPatchLines::leftMousePressed(Vector2 mousePos) {
	bool res = false;

	if (corner != CORNER_NONE) res = true;

	if (res) {
		isResizing = true;
		startMousePos = mousePos;
		heldCorner = corner;
	}

	return res;
}

void NPatchLines::mouseMoved(Vector2 mousePos, int snapWidth, int snapHeight) {
	detectCorner(mousePos);

	if (!isInRect(mousePos)) return;
	if (!isResizing) return;

	Vector2 scaledPos = {mousePos.x / scale, mousePos.y / scale};
	Vector2 scaledTextureSize = {static_cast<float>(texture.width) / scale, static_cast<float>(texture.height) / scale};

	switch (heldCorner) {
		case CORNER_NONE:
			break;
		case CORNER_TOPLEFT:
			info->left = scaledPos.x;
			info->top = scaledPos.y;
			break;
		case CORNER_TOPRIGHT:
			info->right = scaledTextureSize.x - scaledPos.x;
			info->top = scaledPos.y;
			break;
		case CORNER_BOTTOMLEFT:
			info->left = scaledPos.x;
			info->bottom = scaledTextureSize.y - scaledPos.y;
			break;
		case CORNER_BOTTOMRIGHT:
			info->right = scaledTextureSize.x - scaledPos.x;
			info->bottom = scaledTextureSize.y - scaledPos.y;
			break;
	}
}

void NPatchLines::leftMouseReleased(Vector2 mousePos) {
	isResizing = false;
	corner = CORNER_NONE;
	heldCorner = CORNER_NONE;
}