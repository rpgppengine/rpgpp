#include "prop.hpp"

#include <raylib.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "game.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "tilemap.hpp"

using json = nlohmann::json;

Prop::Prop() : worldPos(Vector2{}), tilePos(Vector2{}), atlasRect(), texture(), collisionRect() {}

Prop::Prop(const std::string &filePath) {
	this->sourcePath = filePath;
	this->worldPos = Vector2{0, 0};
	this->tilePos = Vector2{0, 0};

	char *jsonString = LoadFileText(filePath.c_str());
	json json = json::parse(jsonString);
	std::vector<int> atlasRectVec = json.at("atlas_rect");
	if (atlasRectVec.size() != 4) {
		throw std::runtime_error("Not enough items in atlas_rect.");
	}
	this->atlasRect = Rectangle{static_cast<float>(atlasRectVec[0]), static_cast<float>(atlasRectVec[1]),
								static_cast<float>(atlasRectVec[2]), static_cast<float>(atlasRectVec[3])};

	std::vector<int> collisionRectVec = json.at("collision_rect");
	if (collisionRectVec.size() != 4) {
		throw std::runtime_error("Not enough items in collision_rect.");
	}
	this->collisionRect = Rectangle{static_cast<float>(collisionRectVec[0]), static_cast<float>(collisionRectVec[1]),
									static_cast<float>(collisionRectVec[2]), static_cast<float>(collisionRectVec[3])};

	this->imagePath = json.at("image");
	this->texture = LoadTexture(imagePath.c_str());

	this->hasInteractable = static_cast<bool>(json.at("has_interactable"));

	this->interactable = std::make_unique<Interactable>();
	if (hasInteractable) {
		std::string intType = json.at("interactable_type");
		this->interactable = std::make_unique<Interactable>(intType, tilePos, _RPGPP_TILESIZE);
	}
	UnloadFileText(jsonString);
}

Prop::Prop(Rectangle atlasRect, Vector2 worldPos) : texture() {
	this->sourcePath = "";
	this->atlasRect = atlasRect;
	this->worldPos = worldPos;
	this->tilePos = Vector2{worldPos.x / 16.0f, worldPos.y / 16.0f};
	this->collisionRect = Rectangle{0, 0, 16, 16};
	this->hasInteractable = false;
	this->interactable = std::make_unique<Interactable>();
}

Prop::Prop(PropBin bin) {
	this->sourcePath = bin.name;
	this->worldPos = Vector2{0, 0};
	this->tilePos = Vector2{0, 0};
	this->atlasRect = Rectangle{static_cast<float>(bin.atlasRect.x), static_cast<float>(bin.atlasRect.y),
								static_cast<float>(bin.atlasRect.width), static_cast<float>(bin.atlasRect.height)};
	this->collisionRect =
		Rectangle{static_cast<float>(bin.collisionRect.x), static_cast<float>(bin.collisionRect.y),
				  static_cast<float>(bin.collisionRect.width), static_cast<float>(bin.collisionRect.height)};

	ImageBin imgBin = Game::getBin().images.at(GetFileName(bin.imagePath.c_str()));
	Image img = LoadImageFromMemory(GetFileExtension(bin.imagePath.c_str()), imgBin.data.data(), imgBin.dataSize);
	setTexture(LoadTextureFromImage(img));
	UnloadImage(img);

	this->hasInteractable = bin.hasInteractable;
	this->interactable = std::make_unique<Interactable>();
	if (hasInteractable) {
		this->interactable = std::make_unique<Interactable>(bin.intType, tilePos, _RPGPP_TILESIZE);
	}
}

/*
json Prop::dumpJson() {
	std::vector<int> atlasRectVec = {static_cast<int>(atlasRect.x), static_cast<int>(atlasRect.y),
									 static_cast<int>(atlasRect.width), static_cast<int>(atlasRect.height)};
	std::vector<int> collisionRectVec = {static_cast<int>(collisionRect.x), static_cast<int>(collisionRect.y),
										 static_cast<int>(collisionRect.width), static_cast<int>(collisionRect.height)};

	json j{{"atlas_rect", atlasRectVec},
		   {"collision_rect", collisionRectVec},
		   {"image", imagePath},
		   {"has_interactable", hasInteractable},
		   {"interactable_type", interactable->getType()}};
	return j;
}
*/

std::string Prop::getSourcePath() const { return sourcePath; }

void Prop::setTexture(Texture2D texture_to_set) { this->texture = texture_to_set; }

Texture2D Prop::getTexture() const { return texture; }

void Prop::setTextureFromPath(const std::string &image_path) {
	this->imagePath = image_path;
	this->texture = LoadTexture(image_path.c_str());
}

const char *Prop::getImagePath() const { return this->imagePath.c_str(); }

void Prop::setCollisionRect(Rectangle collision_rect_to_set) { this->collisionRect = collision_rect_to_set; }

void Prop::setWorldTilePos(Vector2 world_pos_to_set, int tileSize) {
	this->worldPos = Vector2{world_pos_to_set.x * tileSize, world_pos_to_set.y * tileSize};
	this->tilePos = world_pos_to_set;
}

void Prop::setWorldPos(Vector2 world_pos_to_set) { this->worldPos = world_pos_to_set; }

Vector2 Prop::getWorldPos() const { return worldPos; }

Vector2 Prop::getWorldTilePos() const { return tilePos; }

void Prop::setAtlasRect(Rectangle atlas_rect_to_set) { this->atlasRect = atlas_rect_to_set; }

Rectangle Prop::getAtlasRect() const { return atlasRect; }

Rectangle Prop::getCollisionRect() const { return collisionRect; }

Rectangle Prop::getWorldCollisionRect() const {
	return Rectangle{worldPos.x + (collisionRect.x * RPGPP_DRAW_MULTIPLIER),
					 worldPos.y + (collisionRect.y * RPGPP_DRAW_MULTIPLIER),
					 collisionRect.width * RPGPP_DRAW_MULTIPLIER, collisionRect.height * RPGPP_DRAW_MULTIPLIER};
}

Vector2 Prop::getCollisionCenter() const {
	Rectangle rect = getWorldCollisionRect();
	return Vector2{rect.x + (rect.width / 2), rect.y + (rect.height / 2)};
}

bool Prop::getHasInteractable() const { return hasInteractable; }
void Prop::setHasInteractable(bool value) { this->hasInteractable = value; }

Interactable *Prop::getInteractable() const {
	if (hasInteractable) {
		return interactable.get();
	} else {
		return nullptr;
	}
}

std::string Prop::getInteractableType() const { return interactable->getType(); }

void Prop::setInteractableType(const std::string &type) {
	if (hasInteractable && interactable->getType() == type) {
		return;
	}
	this->interactable->setType(type);
	this->hasInteractable = true;
}

void Prop::draw() const {
	Rectangle dest = {worldPos.x, worldPos.y, atlasRect.width * RPGPP_DRAW_MULTIPLIER,
					  atlasRect.height * RPGPP_DRAW_MULTIPLIER};
	DrawTexturePro(texture, atlasRect, dest, Vector2{0, 0}, 0.0f, WHITE);

	bool debugDraw = true;

	if (Game::isUsingBin()) {
		debugDraw = Game::getBin().gameSet.debugDraw;
	}

	if (debugDraw) {
		DrawRectangleRec(getWorldCollisionRect(), Fade(RED, 0.5f));
	}
}
