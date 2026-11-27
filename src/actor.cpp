#include "actor.hpp"

#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <cstdio>
#include <fstream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "animation.hpp"
#include "basicTypes.hpp"
#include "cereal/archives/json.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "resource.hpp"
#include "tileset.hpp"
using json = nlohmann::json;

Actor::Actor() : Resource(RPGPP_RESOURCE_ACTOR) {
	// Some default animations
	addAnimation(ANIM_DOWN, {{2, 0}, {3, 0}});
	addAnimation(ANIM_DOWN_IDLE, {{0, 0}, {1, 0}});
	addAnimation(ANIM_UP, {{2, 1}, {3, 1}});
	addAnimation(ANIM_UP_IDLE, {{0, 1}, {1, 1}});
	addAnimation(ANIM_LEFT, {{2, 2}, {3, 2}});
	addAnimation(ANIM_LEFT_IDLE, {{0, 2}, {1, 2}});
	addAnimation(ANIM_RIGHT, {{2, 3}, {3, 3}});
	addAnimation(ANIM_RIGHT_IDLE, {{0, 3}, {1, 3}});
}

void Actor::afterLoad() {
	bool tileSetValid = false;
	auto ts = Game::getResources().getUnpacker().loadT<TileSet>(tileSetSource, tileSetValid);
	if (tileSetValid) {
		ts.afterLoad();
		this->tileSet = ts;
		this->tile = tileSet.getTile({0, 0});
	}
}

void Actor::loadFile() {
	std::ifstream stream(tileSetSource);
	{
		cereal::JSONInputArchive ar(stream);
		ar(tileSet);
	}
	tileSet.loadFile();
	this->tile = tileSet.getTile({0, 0});
}

Actor::Actor(const std::string &fileName) : Actor() {}

Actor::Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos, std::string tileSetSource) : Actor() {
	/*
	this->sourcePath = "";
	this->tilePosition = Vector2{-1, -1};
	this->position = Vector2{0, 0};

	// this->tileSet = std::move(tileSet);
	this->tile = tileSet->getTile(atlasPos);
	this->tileSetSource = std::move(tileSetSource);

	this->frameCounter = 0;
	this->frameSpeed = 2;
	this->currentFrame = 0;
	this->currentAnimation = RPGPP_DOWN_IDLE;

	this->interactable = std::make_unique<Interactable>();

	for (int i = 0; i < 8; i++) {
		animations[i] = std::vector<IVector>();
	}

	// Some default animations
	addAnimationFrames(RPGPP_DOWN, {{2, 0}, {3, 0}});

	addAnimationFrames(RPGPP_DOWN_IDLE, {{0, 0}, {1, 0}});

	addAnimationFrames(RPGPP_UP, {{2, 1}, {3, 1}});

	addAnimationFrames(RPGPP_UP_IDLE, {{0, 1}, {1, 1}});

	addAnimationFrames(RPGPP_LEFT, {{2, 2}, {3, 2}});

	addAnimationFrames(RPGPP_LEFT_IDLE, {{0, 2}, {1, 2}});

	addAnimationFrames(RPGPP_RIGHT, {{2, 3}, {3, 3}});

	addAnimationFrames(RPGPP_RIGHT_IDLE, {{0, 3}, {1, 3}});

	IVector defaultTileAtlasPos = animations[currentAnimation].at(0);
	this->tile = tileSet->getTile(defaultTileAtlasPos.toFloat());

	// Default collision box..
	Vector2 atlasTileSize = this->tileSet.getTileSize();
	this->collisionRect =
		Rectangle{0, (atlasTileSize.y * RPGPP_DRAW_MULTIPLIER) / 2, (atlasTileSize.x * RPGPP_DRAW_MULTIPLIER),
				  (atlasTileSize.y * RPGPP_DRAW_MULTIPLIER) / 2};
						*/
}

Actor::Actor(const ActorBin &bin) : Actor() {
	/*
	this->sourcePath = bin.name;
	this->position = Vector2{0, 0};

	this->frameCounter = 0;
	this->frameSpeed = 2;
	this->currentFrame = 0;
	this->currentAnimation = RPGPP_DOWN_IDLE;

	// tileset
	// this->tileSet = std::make_unique<TileSet>(Game::getBin().tilesets.at(bin.tileSetName));

	// collision
	this->collisionRect = Rectangle{static_cast<float>(bin.collision.x), static_cast<float>(bin.collision.y),
									static_cast<float>(bin.collision.width), static_cast<float>(bin.collision.height)};

	// interactable
	this->interactable = std::make_unique<Interactable>();

	// animations
	for (int i = 0; i < 8; i++) {
		std::vector<IVector> binFrames = bin.animations[i];
		std::vector<std::vector<int>> frames;
		for (int j = 0; j < bin.animations[i].size(); j++) {
			std::vector<int> frameVec;
			frameVec.push_back(binFrames[j].x);
			frameVec.push_back(binFrames[j].y);
			frames.push_back(frameVec);
		}

		addAnimationFrames(static_cast<Direction>(i), frames);
	}
	*/
}

void Actor::unload() const { tileSet.unload(); }

int Actor::getCurrentFrame() const { return this->currentFrame; }

int Actor::getAnimationCount() const { return this->getAnimationRaw(this->currentAnimation).getSize(); }

Direction Actor::getAnimationDirection() const {
	if (dirMap.count(currentAnimation) > 0) {
		return dirMap.at(currentAnimation);
	} else {
		return RPGPP_DOWN_IDLE;
	}
}

IVector Actor::getCurrentAnimationAtlas() const {
	return this->getAnimationRaw(this->currentAnimation).getFrame(currentFrame);
}

IVector Actor::getAnimationAtlasByIdx(int frameIndex) const {
	return this->getAnimationRaw(this->currentAnimation).getFrame(frameIndex);
}

void Actor::setCurrentFrame(int frameIndex) {
	this->currentFrame = frameIndex;

	Animation anim = getAnimationRaw(currentAnimation);

	Vector2 atlasTileSize = tileSet.getTileSize();
	IVector atlasPos = anim.getFrame(currentFrame);
	atlasPos = Vector2{atlasPos.x * atlasTileSize.x, atlasPos.y * atlasTileSize.y};

	// this->tile = tileSet.getTile(atlasPos.toFloat());
	this->tile = tileSet.getTile(getAnimationAtlasByIdx(currentFrame).toFloat());
}

Rectangle Actor::getCurrentAnimationRectangle() {
	const auto &atlasPos = this->getCurrentAnimationAtlas();
	const auto &atlasSize = this->getTileSet().getTileSize();
	return {atlasPos.x * atlasSize.x, atlasPos.y * atlasSize.y, atlasSize.x, atlasSize.y};
}

void Actor::update() {
	if (!isPlaying) return;

	frameCounter++;

	if (frameCounter >= (60 / frameSpeed)) {
		frameCounter = 0;
		currentFrame++;

		if (tempAnimIsPlayed) {
			printf("%i \n", currentFrame);
		}

		if (currentFrame >= this->getAnimationCount()) {
			if (currentFrame >= this->getAnimationCount()) {
				currentFrame = 0;
				if (tempAnimIsPlayed) {
					currentAnimation = lastAnimation;
					tempAnimIsPlayed = false;
				}
			}
		}

		this->setCurrentFrame(currentFrame);
	}
}

void Actor::draw() const {
	// defaults..
	constexpr auto origin = Vector2{0.0f, 0.0f};
	constexpr float rotation = 0.0f;

	// texture
	Texture texture = this->tileSet.getTexture();
	if (!IsTextureValid(texture)) return;

	// actual coordinates
	const Vector2 atlasTileSize = tileSet.getTileSize();
	Vector2 atlasCoords = this->tile.getAtlasCoords().toFloat();

	// build rects
	auto atlasRect =
		Rectangle{atlasCoords.x * atlasTileSize.x, atlasCoords.y * atlasTileSize.y, atlasTileSize.x, atlasTileSize.y};
	auto worldRect = getRect();
	// draw it
	DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);

	bool debugDraw = false;

	if (Game::isUsingBin()) {
		debugDraw = Game::getBin().gameSet.debugDraw;
	}

	if (debugDraw) {
		// draw collision rect..
		Rectangle drawnCollisionRect = getCollisionRect();
		DrawRectanglePro(drawnCollisionRect, origin, rotation, Fade(MAROON, 0.5f));
	}
}

void Actor::setIsPlaying(bool value) { isPlaying = value; }

bool Actor::getIsPlaying() { return isPlaying; }

std::string Actor::getSourcePath() const { return sourcePath; }

Rectangle Actor::getRect() const {
	Vector2 atlasTileSize = tileSet.getTileSize();
	auto result = Rectangle{position.x, position.y, atlasTileSize.x * RPGPP_DRAW_MULTIPLIER,
							atlasTileSize.y * RPGPP_DRAW_MULTIPLIER};
	return result;
}

TileSet &Actor::getTileSet() { return tileSet; }

void Actor::setTileSet(const std::string &newTileSetSource) {
	// tileSet = std::make_unique<TileSet>(newTileSetSource);
	this->tileSetSource = newTileSetSource;
}

TileSetRef Actor::getTileSetSource() { return tileSetSource; }

void Actor::setTileSetSource(const TileSetRef &source) { tileSetSource = source.path; }

void Actor::setPosition(Vector2 newPosition) { this->position = newPosition; }

Vector2 Actor::getPosition() const { return position; }

void Actor::setTilePosition(Vector2 newPosition, Vector2 tileSize) {
	Vector2 actorTileSize = tileSet.getTileSize();
	auto absolutePos =
		Vector2{newPosition.x * tileSize.x * RPGPP_DRAW_MULTIPLIER, newPosition.y * tileSize.y * RPGPP_DRAW_MULTIPLIER};

	float xDiff = 0;
	if ((tileSet.getTileWidth() * RPGPP_DRAW_MULTIPLIER) > (tileSize.x * RPGPP_DRAW_MULTIPLIER)) {
		xDiff = ((tileSet.getTileWidth() * RPGPP_DRAW_MULTIPLIER) - (tileSize.x * RPGPP_DRAW_MULTIPLIER)) / 2;
	}

	auto resultVector = Vector2{absolutePos.x - xDiff, absolutePos.y - ((actorTileSize.y * RPGPP_DRAW_MULTIPLIER) -
																		(tileSize.y * RPGPP_DRAW_MULTIPLIER))};
	this->position = resultVector;
	this->tilePosition = newPosition;
}

Vector2 Actor::getTilePosition() const { return tilePosition; }

void Actor::moveByVelocity(Vector2 velocity) {
	Vector2 resultVector = Vector2Add(position, velocity);

	this->position = resultVector;
}

Rectangle Actor::getCollisionRectWithVelocity(Vector2 velocity) const {
	Vector2 newPos = Vector2Add(position, velocity);
	auto result =
		Rectangle{newPos.x + collisionRect.x, newPos.y + collisionRect.y, collisionRect.width, collisionRect.height};

	return result;
}

Vector2 Actor::getCollisionCenter() const {
	Vector2 result = {position.x + collisionRect.x + (collisionRect.width / 2),
					  position.y + collisionRect.y + (collisionRect.height / 2)};
	return result;
}

void Actor::playAnimation(Direction id) {
	printf("playing.. %i\n", static_cast<int>(id));

	this->lastAnimation = currentAnimation;
	this->tempAnimIsPlayed = true;

	this->currentAnimation = id;
	this->currentFrame = -1;

	frameCounter = (60 / frameSpeed);
}

bool Actor::isTempAnimationPlaying() { return tempAnimIsPlayed; }

void Actor::changeAnimation(const std::string &id) {
	if (currentAnimation != id) {
		this->currentFrame = 0;
		this->currentAnimation = id;

		// change to the first frame of the new animation immidiately
		frameCounter = (60 / frameSpeed);
		tile = tileSet.getTile(getAnimationRaw(id).getFrame(0).toFloat());
	}
}

void Actor::changeAnimationDir(Direction id) {
	for (auto &pair : dirMap) {
		if (pair.second == id) {
			changeAnimation(pair.first);
			break;
		}
	}
}

std::string Actor::getCurrentAnimationId() { return currentAnimation; }

std::map<std::string, Animation> &Actor::getAnimationsRaw() { return animations; }

Animation Actor::getAnimationRaw(const std::string &id) const {
	if (animations.count(id) > 0) {
		return animations.at(id);
	} else {
		return Animation{};
	}
}

Animation &Actor::getAnimationRef(const std::string &id) { return animations.at(id); }

void Actor::addAnimation(const std::string &id, std::vector<IVector> frames) {
	Animation anim;
	for (auto &item : frames) {
		anim.addFrame(item);
	}
	animations.emplace(id, anim);
}

Rectangle Actor::getCollisionRect() const { return collisionRect; }

void Actor::setCollisionRect(const Rectangle &rect) { this->collisionRect = rect; }

Vector2 calcActorTilePos(Vector2 newPosition, Vector2 worldTileSize, TileSet *tileSet) {
	Vector2 actorTileSize = tileSet->getTileSize();
	auto absolutePos = Vector2{newPosition.x * worldTileSize.x * RPGPP_DRAW_MULTIPLIER,
							   newPosition.y * worldTileSize.y * RPGPP_DRAW_MULTIPLIER};

	float xDiff = 0;
	if ((tileSet->getTileWidth() * RPGPP_DRAW_MULTIPLIER) > (worldTileSize.x * RPGPP_DRAW_MULTIPLIER)) {
		xDiff = ((tileSet->getTileWidth() * RPGPP_DRAW_MULTIPLIER) - (worldTileSize.x * RPGPP_DRAW_MULTIPLIER)) / 2;
	}

	auto resultVector = Vector2{absolutePos.x - xDiff, absolutePos.y - ((actorTileSize.y * RPGPP_DRAW_MULTIPLIER) -
																		(worldTileSize.y * RPGPP_DRAW_MULTIPLIER))};
	return resultVector;
}

bool Actor::hasInteractable() { return ownsInteractable; }

void Actor::setHasInteractable(bool value) { ownsInteractable = value; }

Interactable *Actor::getInteractable() { return interactable.get(); }

void Actor::setInteractableFromPath(const std::string &path) {
	interactable = std::make_unique<Interactable>(path);
	ownsInteractable = true;
}
