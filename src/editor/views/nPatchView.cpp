#include "views/nPatchView.hpp"

#include <memory>

#include "TGUI/Vector2.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

NPatchView::NPatchView() {}

NPatchView::Ptr NPatchView::create() {
	auto ptr = std::make_shared<NPatchView>();
	return ptr;
}

void NPatchView::setup(NPatchInfo *info, Texture texture, int scale) {
	this->info = info;

	Image image = LoadImageFromTexture(texture);
	ImageResizeNN(&image, texture.width * scale, texture.height * scale);
	this->texture = LoadTextureFromImage(image);
	this->scale = scale;

	lines.info = info;
	lines.texture = this->texture;
	lines.scale = scale;
}

void NPatchView::drawOverlay() {}

void NPatchView::drawCanvas() {
	ClearBackground(RAYWHITE);

	if (!IsTextureValid(texture)) {
		DrawText("Texture Load Failure...", 0, 0, 20, RED);
		return;
	}

	DrawTextureV(texture, {0, 0}, WHITE);
	DrawRectangleLines(0, 0, static_cast<int>(texture.width), static_cast<int>(texture.height), GRAY);

	lines.draw();
}

bool NPatchView::leftMousePressed(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	if (lines.leftMousePressed(mousePos)) {
	}

	return WorldView::leftMousePressed(pos);
}

void NPatchView::leftMouseReleased(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	lines.leftMouseReleased(mousePos);
	onInfoChanged.emit(this, info);

	WorldView::leftMouseReleased(pos);
}

void NPatchView::mouseMoved(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	lines.mouseMoved(mousePos, scale, scale);

	WorldView::mouseMoved(pos);
}