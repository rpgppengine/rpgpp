#include "player.hpp"

#include <raylib.h>
#include <raymath.h>

#include <stdexcept>

#include "conversion.hpp"
#include "game.hpp"
#include "interactable.hpp"

Player::Player(std::unique_ptr<Actor> actor, Room &room) : room(room) {
	this->lock = false;
	this->position = Vector2{0, 0};
	this->velocity = Vector2{0, 0};
	this->size = 48;

	Rectangle collisionRect = actor->getCollisionRect(Vector2{0, 0});
	this->interactableArea =
		Rectangle{collisionRect.x - 6, collisionRect.y - 6, collisionRect.width + 12, collisionRect.height + 12};

	this->actor = std::move(actor);
	this->idleDirection = RPGPP_DOWN_IDLE;
	this->currentDirection = RPGPP_DOWN;

	this->position = this->actor->getPosition();
}

void Player::unload() const { actor->unload(); }

void Player::update() {
	Rectangle collisionRect = actor->getCollisionRect(Vector2{0, 0});
	this->interactableArea =
		Rectangle{collisionRect.x - 6, collisionRect.y - 6, collisionRect.width + 12, collisionRect.height + 12};

	int change = 2;
	velocity = Vector2{0, 0};

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		change *= 2;
	}

	if (IsKeyDown(KEY_UP)) {
		currentDirection = RPGPP_UP;
		idleDirection = RPGPP_UP_IDLE;
		velocity.y -= change;
	} else if (IsKeyDown(KEY_DOWN)) {
		currentDirection = RPGPP_DOWN;
		idleDirection = RPGPP_DOWN_IDLE;
		velocity.y += change;
	} else if (IsKeyDown(KEY_LEFT)) {
		currentDirection = RPGPP_LEFT;
		idleDirection = RPGPP_LEFT_IDLE;
		velocity.x -= change;
	} else if (IsKeyDown(KEY_RIGHT)) {
		currentDirection = RPGPP_RIGHT;
		idleDirection = RPGPP_RIGHT_IDLE;
		velocity.x += change;
	} else if (IsKeyPressed(KEY_Z)) {
		this->handleInteraction();
	}

	if (!lock) this->handleCollision();

	if (lock) return;

	if (!actor->isTempAnimationPlaying()) {
		if (Vector2Equals(velocity, Vector2{0, 0})) {
			actor->changeAnimation(idleDirection);
		} else {
			actor->changeAnimation(currentDirection);
		}
	}

	this->moveByVelocity(velocity);

	actor->update();
}

void Player::draw() const {
	actor->draw();

	bool debugDraw = true;

	if (Game::isUsingBin()) {
		debugDraw = Game::getBin().gameSet.debugDraw;
	}

	if (debugDraw) {
		// debug draw interactable area..
		Color interactableAreaDebugColor = ORANGE;
		interactableAreaDebugColor.a = (255 / 4);

		DrawRectangleRec(interactableArea, interactableAreaDebugColor);
	}
}

void Player::handleCollision() {
	if (actor == nullptr) return;

	Rectangle playerRect = actor->getCollisionRect(velocity);

	TileMap *tileMap = room.getTileMap();
	int worldTileSize = tileMap->getWorldTileSize();

	// collision tiles
	for (auto &[pos, obj] : room.getCollisions().getObjects()) {
		Vector2 v = fromIVector(pos);
		Rectangle tileRect = Rectangle{v.x * worldTileSize, v.y * worldTileSize, static_cast<float>(worldTileSize),
									   static_cast<float>(worldTileSize)};

		if (CheckCollisionRecs(playerRect, tileRect)) {
			velocity = Vector2{0, 0};
			break;
		}
	}

	// props
	for (auto &[pos, prop] : room.getProps().getObjects()) {
		if (CheckCollisionRecs(playerRect, prop->getWorldCollisionRect())) {
			velocity = Vector2{0, 0};
			break;
		}
	}

	// actors
	for (auto &[name, actor] : room.getActors().getActors()) {
		if (CheckCollisionRecs(playerRect, actor->getCollisionRect(Vector2{0, 0}))) {
			velocity = Vector2{0, 0};
			break;
		}
	}

	// interactable tiles
	std::vector<Interactable *> interactableTiles = this->room.getInteractables().getList();
	for (Interactable *interactable : interactableTiles) {
		Rectangle tileRect =
			Rectangle{interactable->getWorldPos().x * room.getWorldTileSize(),
					  interactable->getWorldPos().y * room.getWorldTileSize(),
					  static_cast<float>(room.getWorldTileSize()), static_cast<float>(room.getWorldTileSize())};

		if (CheckCollisionRecs(playerRect, tileRect)) {
			velocity = Vector2{0, 0};

			if (interactable->isOnTouch()) {
				interactable->interact();
				if (interactable->getType() == "warper") {
					lock = true;
					room.setLock(true);
				}
			}

			break;
		}
	}
}

void Player::handleInteraction() {
	std::vector<Interactable *> interactableTiles = this->room.getInteractables().getList();
	for (Interactable *interactable : interactableTiles) {
		Rectangle tileRect =
			Rectangle{interactable->getWorldPos().x * room.getWorldTileSize(),
					  interactable->getWorldPos().y * room.getWorldTileSize(),
					  static_cast<float>(room.getWorldTileSize()), static_cast<float>(room.getWorldTileSize())};

		if (CheckCollisionRecs(interactableArea, tileRect)) {
			if (interactable->getType() == "warper") {
				lock = true;
				room.setLock(true);
			}

			interactable->interact();

			break;
		}
	}

	for (auto &[pos, prop] : room.getProps().getObjects()) {
		if (prop->getHasInteractable()) {
			if (CheckCollisionRecs(interactableArea, prop->getWorldCollisionRect())) {
				prop->getInteractable()->interact();
				break;
			}
		}
	}

	for (auto &[name, actor] : room.getActors().getActors()) {
		if (actor->hasInteractable()) {
			if (CheckCollisionRecs(interactableArea, actor->getCollisionRect(Vector2{0, 0}))) {
				actor->getInteractable()->interact();
				break;
			}
		}
	}
}

void Player::setRoom(Room &room) const {
	// this->room = Room(room);
}

Actor &Player::getActor() const { return *actor; }

void Player::moveByVelocity(Vector2 velocity) {
	Vector2 resultVector = Vector2Add(position, velocity);
	position = resultVector;

	actor->moveByVelocity(velocity);
}

Vector2 Player::getPosition() const {
	if (actor == nullptr) return Vector2{0, 0};

	return actor->getPosition();
}

void Player::setPosition(Vector2 pos) {
	actor->setPosition(pos);
	position = pos;
}

Vector2 Player::getCenterPosition() const {
	if (actor == nullptr) return Vector2{0, 0};

	Rectangle actorRect = actor->getRect();
	return Vector2{actorRect.x + (actorRect.width / 2), actorRect.y + (actorRect.height / 2)};
}

Vector2 Player::getTilePosition() const { return actor->getTilePosition(); }

void Player::setTilePosition(Vector2 tilePos) {
	if (!room.getTileMap()->worldPosIsValid(tilePos)) {
		throw std::runtime_error(TextFormat("This world tile position does not exist: %i, %i",
											static_cast<int>(tilePos.x), static_cast<int>(tilePos.y)));
	}
	actor->setTilePosition(tilePos, room.getTileMap()->getTileSet()->getTileSize());
}

Vector2 Player::getCollisionPos() const {
	if (actor == nullptr) return Vector2{0, 0};

	return actor->getCollisionCenter();
}

Vector2 Player::getCollisionCenterPos() const {
	if (actor == nullptr) return Vector2{0, 0};

	return actor->getCollisionCenter();
}

void Player::setMovementLock(bool value) { lock = value; }
