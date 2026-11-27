#include "utils/gridDraw.hpp"

#include <cmath>

#include "basicTypes.hpp"
#include "drawHelper.hpp"
#include "raylib.h"

using namespace editor;

void GridDraw::draw() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Rectangle tileRect = {static_cast<float>((i * tileWidth)), static_cast<float>(j * tileHeight),
								  static_cast<float>(tileWidth), static_cast<float>(tileHeight)};
			DrawRectangleLinesPro(tileRect, color);
		}
	}
}

IVector GridDraw::getTileAtPosition(Vector2 position) {
	IVector result = {-1, -1};
	if (position.x < (tileWidth * rows) && position.y < (tileHeight * columns)) {
		result.x = std::floor(position.x / (tileWidth));
		result.y = std::floor(position.y / (tileHeight));
	}
	return result;
}

Rectangle GridDraw::getTileRect(IVector pos) {
	Rectangle tileRect = {static_cast<float>((pos.x * tileWidth)), static_cast<float>(pos.y * tileHeight),
						  static_cast<float>(tileWidth), static_cast<float>(tileHeight)};
	return tileRect;
}
