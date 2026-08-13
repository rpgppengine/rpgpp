#include "edui/canvas.hpp"
#include <memory>
#include "edui/rliconsarr.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Canvas::Canvas() {
	render = std::make_unique<CanvasRender>();

	camera = {};
	camera.offset = Vector2{0, 0};
	camera.rotation = 0.0f;
	camera.target = Vector2{0, 0.0f};
	camera.zoom = 1.0f;
}

void Canvas::calcRect(Rectangle& base) {
	Rectangle old = rect;
	Widget::calcRect(base);

	if (old.width != rect.width || old.height != rect.height) {
		if (IsTextureValid(texture.texture)) {
			UnloadRenderTexture(texture);
		}
		texture = LoadRenderTexture(rect.width, rect.height);
	}
}

void Canvas::update() {
	BeginTextureMode(texture);
	ClearBackground(RAYWHITE);

	BeginMode2D(camera);
	drawCanvas();

	EndMode2D();
	drawOverlay();

	EndTextureMode();
}

void Canvas::draw() {
	Rectangle sourceRect = {0.0f, 0.0f, static_cast<float>(texture.texture.width), static_cast<float>(-texture.texture.height)};
	//DrawTextureRec(texture.texture, rect, {rect.x, rect.y}, WHITE);
	DrawTexturePro(texture.texture, sourceRect, rect, {0, 0}, 0.0f, WHITE);

	auto& rend = render->as<CanvasRender>();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void Canvas::drawCanvas() {
	ClearBackground(RAYWHITE);
	DrawRectangleRec({0, 0, 50, 50}, GREEN);
}

void Canvas::drawOverlay() {
	DrawFPS(4, 4);
}

void Canvas::mouseMoved(Vector2 mousePos, Vector2 relative) {
	auto widgetPos = {rect.x, rect.y};
	Vector2 mouseDelta = GetMouseDelta();
	Vector2 cameraMoveScale = Vector2Scale(mouseDelta, -1 / camera.zoom);

	mouseWorldPos = GetScreenToWorld2D(relative, camera);

	if (mouseMiddleButton) {
		camera.target = Vector2Add(camera.target, cameraMoveScale);
	}
}

void Canvas::middleMouseClicked() {
	SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
	mouseMiddleButton = true;
}

void Canvas::middleMouseReleased() {
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	mouseMiddleButton = false;
}

void Canvas::scrolled(float wheelMove) {
	printf("%f \n", wheelMove);

	Vector2 widgetPos = {rect.x, rect.y};
	Vector2 mousePos = Vector2Subtract(GetMousePosition(), widgetPos);

	Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, camera);
	camera.offset = mousePos;
	camera.target = mouseWorldPos;

	camera.zoom *= std::sqrt(exp(wheelMove * cameraZoomSpeed));
	camera.zoom = Clamp(camera.zoom, cameraMinZoom, cameraMaxZoom);

	Widget::scrolled(wheelMove);
}
