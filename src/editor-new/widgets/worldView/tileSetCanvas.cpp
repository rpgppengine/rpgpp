#include "widgets/worldView/tileSetCanvas.hpp"

#include "raylib.h"
#include "utils/gridDraw.hpp"

using namespace editor;

TileSetCanvas::TileSetCanvas() {
	camera.zoom = 7.5f;
	cameraMaxZoom = 7.5f;
}

void TileSetCanvas::setTexture(Texture texture) { this->texture = texture; }

GridDraw &TileSetCanvas::getGrid() { return grid; }

void TileSetCanvas::drawOverlay() {
	auto vec = grid.getTileAtPosition(getMouseWorldPos());
	if (vec.valid()) {
		auto format = TextFormat("[%i, %i]", vec.x, vec.y);
		DrawText(format, 4, 4, 36, DARKGRAY);
	}
}

void TileSetCanvas::drawCanvas() {
	if (!IsTextureValid(texture)) {
		DrawText("Invalid Texture", 0, 0, 36, MAROON);
	} else {
		DrawTextureV(texture, {0, 0}, WHITE);
	}

	grid.draw();

	auto vec = grid.getTileAtPosition(getMouseWorldPos());
	if (vec.valid()) {
		auto rect = grid.getTileRect(vec);
		DrawRectangleLinesEx(rect, 2.0f, Fade(DARKGRAY, 0.5f));
	}
}
