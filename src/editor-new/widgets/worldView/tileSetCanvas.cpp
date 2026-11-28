#include "widgets/worldView/tileSetCanvas.hpp"

#include "basicTypes.hpp"
#include "edui/canvas.hpp"
#include "raylib.h"
#include "utils/gridDraw.hpp"

using namespace editor;

TileSetCanvas::TileSetCanvas() {
	camera.zoom = 7.5f;
	cameraMaxZoom = 7.5f;
}

void TileSetCanvas::setTexture(Texture texture) { this->texture = texture; }

GridDraw &TileSetCanvas::getGrid() { return grid; }

void TileSetCanvas::enableSelection() { selectionMode = true; }

void TileSetCanvas::setSelectedTile(IVector &tile) { selectedTile = tile; }

IVector TileSetCanvas::getSelectedTile() { return selectedTile; }

void TileSetCanvas::init(TileSet &tileSet) {
	auto texture = tileSet.getTexture();

	grid.tileWidth = tileSet.getTileWidth();
	grid.tileHeight = tileSet.getTileHeight();
	grid.rows = (texture.width / grid.tileWidth);
	grid.columns = (texture.height / grid.tileHeight);

	this->texture = texture;
}

void TileSetCanvas::update() {
	edui::Canvas::update();

	if (mouseIsInRect()) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && selectionMode) {
			selectedTile = grid.getTileAtPosition(getMouseWorldPos());
		}
	}
}

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

	if (selectionMode) {
		if (selectedTile.valid()) {
			auto rect = grid.getTileRect(selectedTile);
			DrawRectangleRec(rect, Fade(DARKBLUE, 0.25f));
		}
	}

	auto mouseTile = grid.getTileAtPosition(getMouseWorldPos());
	if (mouseTile.valid()) {
		auto rect = grid.getTileRect(mouseTile);
		DrawRectangleLinesEx(rect, 2.0f, Fade(DARKGRAY, 0.5f));
	}
}
