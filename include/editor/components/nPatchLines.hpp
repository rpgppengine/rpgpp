#ifndef _RPGPP_NPATCHLINES_H
#define _RPGPP_NPATCHLINES_H

#include "components/resizeDirection.hpp"
#include "raylib.h"

enum BoxCorners { CORNER_NONE, CORNER_TOPLEFT, CORNER_TOPRIGHT, CORNER_BOTTOMLEFT, CORNER_BOTTOMRIGHT };

class NPatchLines {
private:
	const float HANDLE_RADIUS = 3.0f;
	const float HALF_RADIUS = HANDLE_RADIUS / 2;

	bool isResizing = false;
	Vector2 startMousePos = {0, 0};
	ResizeDirection direction;
	BoxCorners corner;

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