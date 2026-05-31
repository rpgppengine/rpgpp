#ifndef _RPGPP_NPATCHLINES_H
#define _RPGPP_NPATCHLINES_H

#include "components/resizeDirection.hpp"
#include "raylib.h"

enum BoxCorners { CORNER_NONE, CORNER_TOPLEFT, CORNER_TOPRIGHT, CORNER_BOTTOMLEFT, CORNER_BOTTOMRIGHT };

class NPatchLines {
private:
	const float HANDLE_RADIUS = 3.0f;

	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;

	bool isResizing = false;
	Vector2 startMousePos = {0, 0};
	BoxCorners corner;
	BoxCorners heldCorner;

	bool isInRect(Vector2 mousePos);
	void calcCorners();
	void detectCorner(Vector2 mousePos);

public:
	Texture texture;
	NPatchInfo *info = nullptr;
	int scale = 1;

	NPatchLines();
	void draw();

	bool leftMousePressed(Vector2 mousePos);
	void mouseMoved(Vector2 mousePos, int snapWidth, int snapHeight);
	void leftMouseReleased(Vector2 mousePos);
};

#endif