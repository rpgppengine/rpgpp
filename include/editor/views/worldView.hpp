#ifndef _RPGPP_WORLDVIEW_H
#define _RPGPP_WORLDVIEW_H

#include <memory>

#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "constants/room.hpp"
#include "raylib.h"
#include "updatable.hpp"

class WorldView : public tgui::CanvasRaylib, public IUpdatable {
protected:
	Camera2D camera;
	RenderTexture texture;
	bool mouseMiddleButton = false;
	bool mouseLeftButton = false;
	Vector2 mouseWorldPos = Vector2{0, 0};
	Vector2 getMouseWorldPos();
	RoomTool tool = RoomTool::TOOL_NONE;

	float cameraZoomSpeed = 0.2f;
	float cameraMinZoom = 0.5f;
	float cameraMaxZoom = 5;
	tgui::Vector2f widgetSize{};

public:
	typedef std::shared_ptr<WorldView> Ptr;
	typedef std::shared_ptr<const WorldView> ConstPtr;

	WorldView(const char *typeName = "WorldView", bool initRenderer = true);
	~WorldView();

	static WorldView::Ptr create();
	static WorldView::Ptr copy(const WorldView::ConstPtr &widget);

	void setSize(const tgui::Layout2d &size) override;
	bool isMouseOnWidget(tgui::Vector2f pos) const override;
	void draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const override;

	void resetRender();

	void mouseMoved(tgui::Vector2f pos) override;
	bool scrolled(float delta, tgui::Vector2f pos, bool touch) override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;

	void keyPressed(const tgui::Event::KeyEvent &event) override;

	bool canGainFocus() const override;

	virtual void drawCanvas();
	virtual void drawOverlay();

	void drawOrigin();

	virtual void update() override;
	static std::shared_ptr<IUpdatable> asUpdatable(const std::shared_ptr<WorldView> &ptr);

	void setTool(RoomTool newTool);
	bool isInView();

protected:
	tgui::Widget::Ptr clone() const override;
};

#endif
