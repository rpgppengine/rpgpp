#include "widgets/worldView/actorCanvas.hpp"

#include "drawHelper.hpp"
#include "edui/canvas.hpp"
#include "raylib.h"

using namespace editor;

ActorCanvas::ActorCanvas() {
	camera.zoom = 7.5f;
	cameraMaxZoom = 7.5f;

	grid.color = Fade(DARKGRAY, 0.25f);
	grid.tileHeight = RPGPP_DRAW_MULTIPLIER;
	grid.tileWidth = RPGPP_DRAW_MULTIPLIER;

	box.setSnap(RPGPP_DRAW_MULTIPLIER);

	box.mouseReleased.connect([this](Rectangle a, Rectangle b) { this->onCollisionChanged.invoke(a, b); });
}

void ActorCanvas::update() {
	edui::Canvas::update();
	if (actor != nullptr) {
		actor->update();
		box.setMousePos(mouseWorldPos);
		box.update();
	}
}

void ActorCanvas::drawCanvas() {
	if (actor == nullptr) return;

	actor->draw();

	grid.draw();
	box.draw();

	DrawRectangleLinesPro(actor->getRect(), DARKGRAY);
}

void ActorCanvas::setActor(Actor *ptr) {
	this->actor = ptr;
	refresh();
}

void ActorCanvas::refresh() {
	box.setRectangle(actor->getCollisionRect());

	grid.rows = actor->getRect().width / grid.tileWidth;
	grid.columns = actor->getRect().height / grid.tileHeight;
}
