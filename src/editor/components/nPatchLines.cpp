#include "components/nPatchLines.hpp"

#include <cmath>
#include <cstdio>

#include "components/resizeDirection.hpp"
#include "raylib.h"

NPatchLines::NPatchLines() {}

void NPatchLines::draw() {
	if (info == nullptr) return;
	if (!IsTextureValid(texture)) return;

	// top and bottom
	DrawLine(0, info->top * scale, texture.width, info->top * scale, RED);
	DrawLine(0, info->bottom * scale, texture.width, info->bottom * scale, RED);

	// left and right
	DrawLine(info->left * scale, 0, info->left * scale, texture.height, RED);
	DrawLine(info->right * scale, 0, info->right * scale, texture.height, RED);

	DrawCircleV({static_cast<float>(info->left * scale), static_cast<float>(info->top * scale)}, 3.0f, MAROON);
	DrawCircleV({static_cast<float>(info->right * scale), static_cast<float>(info->top * scale)}, 3.0f, MAROON);

	DrawCircleV({static_cast<float>(info->left * scale), static_cast<float>(info->bottom * scale)}, 3.0f, MAROON);
	DrawCircleV({static_cast<float>(info->right * scale), static_cast<float>(info->bottom * scale)}, 3.0f, MAROON);
}

bool NPatchLines::leftMousePressed(Vector2 mousePos) {
	direction = NONE;
	corner = CORNER_NONE;

	Vector2 topLeft = {static_cast<float>(info->left * scale), static_cast<float>(info->top * scale)};
	Vector2 topRight = {static_cast<float>(info->right * scale), static_cast<float>(info->top * scale)};
	Vector2 bottomLeft = {static_cast<float>(info->left * scale), static_cast<float>(info->bottom * scale)};
	Vector2 bottomRight = {static_cast<float>(info->right * scale), static_cast<float>(info->bottom * scale)};

	bool res = false;

	if (mousePos.x >= (topLeft.x - HALF_RADIUS) && mousePos.x <= (topLeft.x + HALF_RADIUS) &&
		mousePos.y >= (topLeft.y - HALF_RADIUS) && mousePos.y <= (topLeft.y + HALF_RADIUS)) {
		corner = CORNER_TOPLEFT;
		res = true;
	} else if (mousePos.x >= (topRight.x - HALF_RADIUS) && mousePos.x <= (topRight.x + HALF_RADIUS) &&
			   mousePos.y >= (topRight.y - HALF_RADIUS) && mousePos.y <= (topRight.y + HALF_RADIUS)) {
		corner = CORNER_TOPRIGHT;
		res = true;
	} else if (mousePos.x >= (bottomLeft.x - HALF_RADIUS) && mousePos.x <= (bottomLeft.x + HALF_RADIUS) &&
			   mousePos.y >= (bottomLeft.y - HALF_RADIUS) && mousePos.y <= (bottomLeft.y + HALF_RADIUS)) {
		corner = CORNER_BOTTOMLEFT;
		res = true;
	} else if (mousePos.x >= (bottomRight.x - HALF_RADIUS) && mousePos.x <= (bottomRight.x + HALF_RADIUS) &&
			   mousePos.y >= (bottomRight.y - HALF_RADIUS) && mousePos.y <= (bottomRight.y + HALF_RADIUS)) {
		corner = CORNER_BOTTOMRIGHT;
		res = true;
	}

	if (res) {
		isResizing = true;
		startMousePos = mousePos;
	}

	return res;
}

void NPatchLines::mouseMoved(Vector2 mousePos, int snapWidth, int snapHeight) {
	if (!isResizing) return;

	int dx = std::round((mousePos.x - startMousePos.x) / snapWidth) * snapWidth;
	int dy = std::round((mousePos.y - startMousePos.y) / snapHeight) * snapHeight;

	dx /= scale;
	dy /= scale;

	switch (corner) {
		case CORNER_NONE:
			break;
		case CORNER_TOPLEFT:
			printf("topleft \n");
			info->left = mousePos.x / scale;
			info->top = mousePos.y / scale;
			break;
		case CORNER_TOPRIGHT:
			printf("topright.. \n");
			info->right = mousePos.x / scale;
			info->top = mousePos.y / scale;
			break;
		case CORNER_BOTTOMLEFT:
			printf("bottomleft \n");
			info->left = mousePos.x / scale;
			info->bottom = mousePos.y / scale;
			break;
		case CORNER_BOTTOMRIGHT:
			printf("bottomright \n");
			info->right = mousePos.x / scale;
			info->bottom = mousePos.y / scale;
			break;
	}
}

void NPatchLines::leftMouseReleased(Vector2 mousePos) { corner = CORNER_NONE; }