#include "actor.hpp"

#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <utility>

#include "basicTypes.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "tileset.hpp"
using json = nlohmann::json;

Actor::Actor() {
	for (int i = 0; i < 8; i++) {
		animations[i] = std::vector<IVector>();
	}
}

Actor::Actor(const std::string &fileName) { loadFile(fileName); }

void Actor::loadFile(std::filesystem::path filePath) {
	char *jsonContent = LoadFileText(filePath.c_str());
	json j = json::parse(jsonContent);
	UnloadFileText(jsonContent);

	std::vector<int> collisionInfo = j.at("collision");
	int x = collisionInfo.at(0);
	int y = collisionInfo.at(1);
	int width = collisionInfo.at(2);
	int height = collisionInfo.at(3);

	this->collisionRect =
		Rectangle{static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};

	this->tileSetSource = j.at("tileset");
	this->tileSet = std::make_unique<TileSet>(tileSetSource);

	this->interactable = std::make_unique<Interactable>();

	std::vector<std::vector<int>> down = j.at("animations").at("down");
	addAnimationFrames(RPGPP_DOWN, down);

	std::vector<std::vector<int>> downIdle = j.at("animations").at("down-idle");
	addAnimationFrames(RPGPP_DOWN_IDLE, downIdle);

	std::vector<std::vector<int>> up = j.at("animations").at("up");
	addAnimationFrames(RPGPP_UP, up);

	std::vector<std::vector<int>> upIdle = j.at("animations").at("up-idle");
	addAnimationFrames(RPGPP_UP_IDLE, upIdle);

	std::vector<std::vector<int>> left = j.at("animations").at("left");
	addAnimationFrames(RPGPP_LEFT, left);

	std::vector<std::vector<int>> leftIdle = j.at("animations").at("left-idle");
	addAnimationFrames(RPGPP_LEFT_IDLE, leftIdle);

	std::vector<std::vector<int>> right = j.at("animations").at("right");
	addAnimationFrames(RPGPP_RIGHT, right);

	std::vector<std::vector<int>> rightIdle = j.at("animations").at("right-idle");
	addAnimationFrames(RPGPP_RIGHT_IDLE, rightIdle);

	IVector defaultTileAtlasPos = animations[currentAnimation].at(0);
	this->tile = tileSet->getTile(defaultTileAtlasPos.toFloat());
}

Actor::Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos, std::string tileSetSource) {
	this->sourcePath = "";
	this->tilePosition = Vector2{-1, -1};
	this->position = Vector2{0, 0};

	this->tileSet = std::move(tileSet);
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
	Vector2 atlasTileSize = this->tileSet->getTileSize();
	this->collisionRect =
		Rectangle{0, (atlasTileSize.y * RPGPP_DRAW_MULTIPLIER) / 2, (atlasTileSize.x * RPGPP_DRAW_MULTIPLIER),
				  (atlasTileSize.y * RPGPP_DRAW_MULTIPLIER) / 2};
}

Actor::Actor(const ActorBin &bin) {
	this->sourcePath = bin.name;
	this->position = Vector2{0, 0};

	this->frameCounter = 0;
	this->frameSpeed = 2;
	this->currentFrame = 0;
	this->currentAnimation = RPGPP_DOWN_IDLE;

	// tileset
	this->tileSet = std::make_unique<TileSet>(Game::getBin().tilesets.at(bin.tileSetName));

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
}

/*
json Actor::dumpJson() {
	std::vector<int> collisionVec;
	collisionVec.push_back(static_cast<int>(collisionRect.x));
	collisionVec.push_back(static_cast<int>(collisionRect.y));
	collisionVec.push_back(static_cast<int>(collisionRect.width));
	collisionVec.push_back(static_cast<int>(collisionRect.height));

	std::map<std::string, std::vector<std::vector<int>>, std::less<>> animsVec;

	for (int i = 0; i < 8; i++) {
		std::vector<std::vector<int>> framesVec;
		std::vector<Vector2> animFrames = animations[i];

		for (Vector2 frameVector : animFrames) {
			std::vector<int> outFrame;
			outFrame.push_back(static_cast<int>(frameVector.x));
			outFrame.push_back(static_cast<int>(frameVector.y));
			framesVec.push_back(outFrame);
		}

		auto direction = static_cast<Direction>(i);
		std::string keyName = "untitled";
		switch (direction) {
			case RPGPP_DOWN_IDLE:
				keyName = "down-idle";
				break;
			case RPGPP_DOWN:
				keyName = "down";
				break;
			case RPGPP_UP_IDLE:
				keyName = "up-idle";
				break;
			case RPGPP_UP:
				keyName = "up";
				break;
			case RPGPP_LEFT_IDLE:
				keyName = "left-idle";
				break;
			case RPGPP_LEFT:
				keyName = "left";
				break;
			case RPGPP_RIGHT_IDLE:
				keyName = "right-idle";
				break;
			case RPGPP_RIGHT:
				keyName = "right";
				break;
		}

		animsVec[keyName] = framesVec;
	}

	json j = {{"tileset", tileSetSource}, {"collision", collisionVec}, {"animations", animsVec}};

	return j;
}
*/

void Actor::unload() const { tileSet->unload(); }

int Actor::getCurrentFrame() const { return this->currentFrame; }

int Actor::getAnimationCount() const { return this->getAnimationRaw(this->currentAnimation).size(); }

Direction Actor::getAnimationDirection() const { return this->currentAnimation; }

IVector Actor::getCurrentAnimationAtlas() const {
	return this->getAnimationRaw(this->currentAnimation).at(currentFrame);
}

IVector Actor::getAnimationAtlasByIdx(int frameIndex) const {
	return this->getAnimationRaw(this->currentAnimation).at(frameIndex);
}

void Actor::setAnimationFrame(int frameIndex) {
	this->currentFrame = frameIndex;

	Vector2 atlasTileSize = tileSet->getTileSize();
	IVector atlasPos = animations[currentAnimation].at(currentFrame);
	atlasPos = Vector2{atlasPos.x * atlasTileSize.x, atlasPos.y * atlasTileSize.y};

	if (this->onFrameChanged != nullptr) this->onFrameChanged(this->currentFrame);

	this->tile = tileSet->getTile(atlasPos.toFloat());
}

Rectangle Actor::getCurrentAnimationRectangle() const {
	const auto &atlasPos = this->getCurrentAnimationAtlas();
	const auto &atlasSize = this->getTileSet().getTileSize();
	return {atlasPos.x * atlasSize.x, atlasPos.y * atlasSize.y, atlasSize.x, atlasSize.y};
}

void Actor::update() {
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

		this->setAnimationFrame(currentFrame);
	}
}

void Actor::draw() const {
	// defaults..
	constexpr auto origin = Vector2{0.0f, 0.0f};
	constexpr float rotation = 0.0f;
	const Vector2 atlasTileSize = tileSet->getTileSize();
	auto worldTileSize = Vector2{atlasTileSize.x * RPGPP_DRAW_MULTIPLIER, atlasTileSize.y * RPGPP_DRAW_MULTIPLIER};

	// texture
	Texture texture = this->tileSet->getTexture();

	// actual coordinates
	Vector2 atlasCoords = this->tile.getAtlasCoords().toFloat();

	// build rects
	auto atlasRect = Rectangle{atlasCoords.x, atlasCoords.y, atlasTileSize.x, atlasTileSize.y};
	auto worldRect = Rectangle{position.x, position.y, worldTileSize.x, worldTileSize.y};
	// draw it
	DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);

	bool debugDraw = true;

	if (Game::isUsingBin()) {
		debugDraw = Game::getBin().gameSet.debugDraw;
	}

	if (debugDraw) {
		// draw collision rect..
		auto collisionDebugColor = GRAY;
		collisionDebugColor.a = (255 / 2);

		Rectangle drawnCollisionRect = getCollisionRect(Vector2{0, 0});
		DrawRectanglePro(drawnCollisionRect, origin, rotation, collisionDebugColor);

		// let me draw the collision center
		DrawCircleV(getCollisionCenter(), 2.5f, MAROON);
	}
}

std::string Actor::getSourcePath() const { return sourcePath; }

Rectangle Actor::getRect() const {
	if (tileSet == nullptr) return Rectangle{0, 0, 0, 0};

	Vector2 atlasTileSize = tileSet->getTileSize();
	auto result = Rectangle{position.x, position.y, atlasTileSize.x * RPGPP_DRAW_MULTIPLIER,
							atlasTileSize.y * RPGPP_DRAW_MULTIPLIER};
	return result;
}

TileSet &Actor::getTileSet() const { return *tileSet; }

void Actor::setTileSet(const std::string &newTileSetSource) {
	tileSet = std::make_unique<TileSet>(newTileSetSource);
	this->tileSetSource = newTileSetSource;
}

void Actor::setPosition(Vector2 newPosition) { this->position = newPosition; }

Vector2 Actor::getPosition() const { return position; }

void Actor::setTilePosition(Vector2 newPosition, Vector2 tileSize) {
	Vector2 actorTileSize = tileSet->getTileSize();
	auto absolutePos =
		Vector2{newPosition.x * tileSize.x * RPGPP_DRAW_MULTIPLIER, newPosition.y * tileSize.y * RPGPP_DRAW_MULTIPLIER};

	float xDiff = 0;
	if ((tileSet->getTileWidth() * RPGPP_DRAW_MULTIPLIER) > (tileSize.x * RPGPP_DRAW_MULTIPLIER)) {
		xDiff = ((tileSet->getTileWidth() * RPGPP_DRAW_MULTIPLIER) - (tileSize.x * RPGPP_DRAW_MULTIPLIER)) / 2;
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

Rectangle Actor::getCollisionRect(Vector2 velocity) const {
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

void Actor::addAnimationFrame(Direction id, Vector2 atlasPos) { animations[id].push_back(atlasPos); }

void Actor::removeAnimationFrame(Direction id, int frameIndex) {
	if (currentFrame == frameIndex) {
		currentFrame = 0;
	}
	if (frameIndex == 0) return;
	// how do we know the frame was valid on index side?
	assert(frameIndex < animations[id].size() && "requested animation index is invalid");
	animations[id].erase(animations[id].begin() + frameIndex);
}

void Actor::setAnimationFrame(Direction id, int frameIndex, Vector2 atlasTile) {
	animations[id][frameIndex] = atlasTile;
}

void Actor::addAnimationFrames(const Direction id, const std::vector<std::vector<int>> &frames) {
	for (int i = 0; i < frames.size(); i++) {
		int x = frames.at(i).at(0);
		int y = frames.at(i).at(1);
		animations[id].push_back(Vector2{static_cast<float>(x), static_cast<float>(y)});
	}
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

void Actor::changeAnimation(Direction id) {
	if (this->currentAnimation != id) {
		this->currentFrame = 0;
		this->currentAnimation = id;

		// change to the first frame of the new animation immidiately
		frameCounter = (60 / frameSpeed);
	}
}

std::array<std::vector<IVector>, 8> Actor::getAnimationsRaw() const { return animations; }

std::vector<IVector> Actor::getAnimationRaw(const Direction id) const {
	if (animations[id].empty()) {
		return {};
	}
	std::vector<IVector> animFrames = animations[id];
	return animFrames;
}

std::string Actor::getTileSetSource() const { return tileSetSource; }

Rectangle Actor::getCollisionRect() const { return collisionRect; }

void Actor::setCollisionRect(Rectangle rect) { this->collisionRect = rect; }

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
