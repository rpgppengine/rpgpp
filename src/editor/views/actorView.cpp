#include "views/actorView.hpp"

#include <iostream>
#include <memory>

#include "TGUI/Vector2.hpp"
#include "actor.hpp"
#include "components/resizableCanvasBox.hpp"
#include "fileViews/actorFileView.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "tileset.hpp"
#include "views/worldView.hpp"

constexpr float DELTATIME_DIFFERENCE = 100.0f;

ActorView::ActorView(ActorFileView *actorFileView) : actorFileView(actorFileView) {
	camera.zoom = 5.0f;
	cameraMaxZoom = 10.0f;
}

ActorView::Ptr ActorView::create(ActorFileView *actorFileView) { return std::make_shared<ActorView>(actorFileView); }

ActorView::Ptr ActorView::create(ActorFileView *actorFileView, Actor *actor) {
	auto ptr = std::make_shared<ActorView>(actorFileView);
	ptr->setActor(actor);
	return ptr;
}

void ActorView::setActor(Actor *actor) {
	this->actor = actor;

	this->collisionBox = std::make_unique<ResizableCanvasBox>("collisionRect", this->actor->getCollisionRect(), RED);

	this->atlasBox =
		std::make_unique<ResizableCanvasBox>("atlasRect", actor->getCurrentAnimationRectangle(), BLUE, false);
	this->onAttributeChanged.emit(this);

	actor->resetAnimation();
}

void ActorView::mouseMoved(tgui::Vector2f pos) {
	if (this->actor == nullptr) return;

	const auto &mousePos = getMouseWorldPos();

	if (collisionBox->mouseMovedTrigger(mousePos, RPGPP_DRAW_MULTIPLIER, RPGPP_DRAW_MULTIPLIER)){
		this->onAttributeChanged.emit(this);
	}

	const auto &tileSize = this->actor->getTileSet().getTileSize();

	if (atlasBox->mouseMovedTrigger(mousePos, tileSize.x, tileSize.y)){
		this->onAttributeChanged.emit(this);
	}

	WorldView::mouseMoved(pos);
}

bool ActorView::leftMousePressed(tgui::Vector2f pos) {
	if (this->actor == nullptr) return false;

	const auto &mousePos = getMouseWorldPos();

	if (collisionBox->leftMousePressed(mousePos) && !this->editData) collisionBox->focused = true;

	if (atlasBox->leftMousePressed(mousePos) && this->editData) atlasBox->focused = true;

	return WorldView::leftMousePressed(pos);
}

void ActorView::setCollisionRect(const Rectangle &collision) {
	this->actor->setCollisionRect(collision);
	this->collisionBox->updateRec(collision);
	this->onAttributeChanged.emit(this);
}

void ActorView::leftMouseReleased(tgui::Vector2f pos) {
	if (this->actor == nullptr) return;

	const auto &mousePos = getMouseWorldPos();
	actor->setCollisionRect(collisionBox->leftMouseReleased(mousePos));
	collisionBox->focused = false;

	const auto &tileSize = this->actor->getTileSet().getTileSize();
	const auto &rect = atlasBox->leftMouseReleased(mousePos);

	this->actor->setAnimationFrame(this->actor->getAnimationDirection(), this->actor->getCurrentFrame(),
								   {rect.x / tileSize.x, rect.y / tileSize.y});

	actorFileView->frameEditor->updateFrameButtons();

	atlasBox->focused = false;
	WorldView::leftMouseReleased(pos);
}

void ActorView::setAtlasRect(const Rectangle &rect) {
	this->atlasBox->updateRec(rect);
}

void ActorView::drawCanvas() {
	if (actor == nullptr || collisionBox == nullptr) {
		std::cerr << "Importing Error!" << std::endl;
		return;
	}

	TileSet &tileSet = actor->getTileSet();

	if (animationCurrentDuration > animationFrameDuration / DELTATIME_DIFFERENCE && isPlaying) {
		actor->update();
		animationCurrentDuration = 0.0f;
	}
	this->animationCurrentDuration += GetFrameTime();

	ClearBackground(RAYWHITE);
	if (!this->editData) {
		actor->draw();
		collisionBox->draw();
	} else {
		DrawTexture(tileSet.getTexture(), 0, 0, WHITE);
		atlasBox->draw();
	}
	this->drawOrigin();

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}
