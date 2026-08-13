#ifndef _EDUI_CANVAS_H
#define _EDUI_CANVAS_H

#include "edui/widget.hpp"
namespace edui {
struct CanvasRender : public WidgetRender {};

struct Canvas : public Widget {
	Canvas();

	void calcRect(Rectangle &base) override;

	void update() override;
	void draw() override;

	virtual void drawCanvas();
	virtual void drawOverlay();

	void mouseMoved(Vector2 mousePos, Vector2 relative) override;
	void middleMouseClicked() override;
	void middleMouseReleased() override;
	void scrolled(float wheelMove) override;

protected:
	Camera2D camera;
	RenderTexture texture;
	bool mouseMiddleButton = false;
	bool mouseLeftButton = false;
	Vector2 mouseWorldPos = Vector2{0, 0};
	Vector2 getMouseWorldPos();
	// RoomTool tool = RoomTool::TOOL_NONE;

	float cameraZoomSpeed = 0.2f;
	float cameraMinZoom = 0.5f;
	float cameraMaxZoom = 5;
};
}  // namespace edui

#endif
