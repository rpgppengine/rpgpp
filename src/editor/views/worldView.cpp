#include "views/worldView.hpp"

#include <TGUI/Backend/Renderer/Raylib/BackendRendererRaylib.hpp>
#include <TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp>
#include <TGUI/Backend/Window/Backend.hpp>
#include <cmath>
#include <cstdio>
#include <memory>

#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Event.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "raylib.h"
#include "raymath.h"
#include "updatable.hpp"

// TODO: Possible memory leak (Load with no Unload visible)
WorldView::WorldView(const char *typeName, bool initRenderer) : tgui::CanvasRaylib(typeName, initRenderer) {
	mouseMiddleButton = false;
	mouseWorldPos = {0, 0};

	tgui::Vector2f size = getSize();
	widgetSize = size;

	camera = {};
	camera.offset = Vector2{0, 0};
	camera.rotation = 0.0f;
	camera.target = Vector2{0, 0.0f};
	camera.zoom = 1.0f;

	tool = RoomTool::TOOL_NONE;
}

WorldView::~WorldView() {}

WorldView::Ptr WorldView::create() { return std::make_shared<WorldView>(); }

WorldView::Ptr WorldView::copy(const ConstPtr &widget) {
	if (widget) {
		return std::static_pointer_cast<WorldView>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr WorldView::clone() const { return std::make_shared<WorldView>(*this); }

void WorldView::setSize(const tgui::Layout2d &size) { tgui::CanvasRaylib::setSize(size); }

bool WorldView::isMouseOnWidget(tgui::Vector2f pos) const {
	return tgui::FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos);
}

void WorldView::mouseMoved(tgui::Vector2f pos) {
	auto widgetPos = getPosition();
	Vector2 mouseDelta = GetMouseDelta();
	Vector2 cameraMoveScale = Vector2Scale(mouseDelta, -1 / camera.zoom);

	Vector2 mousePos = {pos.x, pos.y};
	mousePos = Vector2Subtract(mousePos, Vector2{widgetPos.x, widgetPos.y});
	mouseWorldPos = GetScreenToWorld2D(mousePos, camera);

	if (mouseMiddleButton) {
		camera.target = Vector2Add(camera.target, cameraMoveScale);
	}

	tgui::Widget::mouseMoved(pos);
}

constexpr int MAXIMUM_LINE = 100;

void WorldView::drawOrigin() {
	// since this function is used to draw the origin x-y axis, it's been moved
	// here.

	DrawLine(0, -MAXIMUM_LINE, 0, MAXIMUM_LINE, DARKGREEN);
	DrawLine(-MAXIMUM_LINE, 0, MAXIMUM_LINE, 0, RED);

	DrawCircle(0, 0, 1.0f, RED);
}

bool WorldView::scrolled(float delta, tgui::Vector2f pos, bool touch) {
	if (!touch) {
		auto widgetPos = getPosition();
		Vector2 mousePos = {pos.x, pos.y};
		mousePos = Vector2Subtract(mousePos, Vector2{widgetPos.x, widgetPos.y});
		Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, camera);
		camera.offset = mousePos;
		camera.target = mouseWorldPos;

		camera.zoom *= std::sqrt(exp(delta * cameraZoomSpeed));
		camera.zoom = Clamp(camera.zoom, cameraMinZoom, cameraMaxZoom);
	}

	return Widget::scrolled(delta, pos, touch);
}

bool WorldView::leftMousePressed(tgui::Vector2f pos) {
	mouseLeftButton = true;
	return tgui::CanvasRaylib::leftMousePressed(pos);
}

void WorldView::leftMouseReleased(tgui::Vector2f pos) { mouseLeftButton = false; }

void WorldView::keyPressed(const tgui::Event::KeyEvent &event) {
	if (event.control && event.code == tgui::Event::KeyboardKey::Z) {
		printf("let me undo.. \n");
	}
}

bool WorldView::canGainFocus() const { return true; }

void WorldView::update() {
	mouseMiddleButton = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

	setMouseCursor(mouseMiddleButton ? tgui::Cursor::Type::Move : tgui::Cursor::Type::Arrow);

	BeginTextureMode(m_textureTarget);

	BeginMode2D(camera);
	drawCanvas();

	EndMode2D();
	drawOverlay();

	EndTextureMode();
}

void WorldView::drawCanvas() { ClearBackground(RAYWHITE); }

void WorldView::drawOverlay() {}

void WorldView::draw(tgui::BackendRenderTarget &target, const tgui::RenderStates states) const {
	constexpr tgui::Borders borders{2};	 // Borders are 2 pixels thick on any side

	CanvasRaylib::draw(target, states);
	target.drawBorders(states, borders, getSize(), tgui::Color::applyOpacity(tgui::Color::Black, 0.5f));
}

Vector2 WorldView::getMouseWorldPos() { return mouseWorldPos; }

std::shared_ptr<IUpdatable> WorldView::asUpdatable(const std::shared_ptr<WorldView> &ptr) {
	return std::dynamic_pointer_cast<IUpdatable>(ptr);
}

void WorldView::setTool(RoomTool newTool) { this->tool = newTool; }

bool WorldView::isInView() {
	return CheckCollisionPointRec(GetMousePosition(), {getPosition().x, getPosition().y, getSize().x, getSize().y});
}
