#ifndef _RPGPP_PROP_H
#define _RPGPP_PROP_H

#include <raylib.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "cereal/access.hpp"
#include "cereal/cereal.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "resource.hpp"
#include "saveable.hpp"
#include "tilemap.hpp"

using json = nlohmann::json;

/** A Prop is an object in the world that has an image and optionally has an Interactable. */
class Prop : public Resource {
private:
	/** Path to the file, used for instantiating this Prop. */
	std::string sourcePath;
	/** Absolute world position of this Prop. */
	Vector2 worldPos = {0, 0};
	/** Position of this Prop, defined in tiles. */
	Vector2 tilePos = {0, 0};
	/** Atlas Rectangle which defines which part of the image is used for drawing this Prop. */
	Rectangle atlasRect;
	/** Texture instance which is used for drawing this Prop. */
	Texture2D texture;
	/** Collision Rectangle which defines where exactly is the collision. */
	Rectangle collisionRect;
	/** Path to the image used for this Prop. */
	std::string imagePath;
	/** Whether this Prop has an Interactable. */
	bool hasInteractable = false;
	/** Smart pointer that owns the Interactable */
	std::unique_ptr<Interactable> interactable;

	friend class cereal::access;

public:
	/** Empty constructor */
	Prop();
	/** Construct from a file. */
	Prop(const std::string &filePath);
	/** Construct from an atlas rect and an absolute world position. */
	Prop(Rectangle atlasRect, Vector2 worldPos);
	/** Construct from a binary structure. */
	explicit Prop(PropBin bin);
	/** Dump the JSON of this Prop. */
	// json dumpJson();
	void applyJson(json j);
	/** Get the source file, used for instantiating this Prop. */
	std::string getSourcePath() const;
	/** Set the Texture for this Prop. */
	void setTexture(Texture2D texture);
	/** Get the Texture for this Prop. */
	Texture2D getTexture() const;
	/** Set the Texture, using an image path. */
	void setTextureFromPath(const std::string &imagePath);
	/** Get the path to the image used for this Prop. */
	const char *getImagePath() const;
	/** Set the collision rectangle. */
	void setCollisionRect(Rectangle collisionRect);
	/** Set the tile position of this Prop. */
	void setWorldTilePos(Vector2 worldPos, int tileSize);
	/** Set the absolute world position of this Prop. */
	void setWorldPos(Vector2 worldPos);
	/** Get the absolute world position of this Prop. */
	Vector2 getWorldPos() const;
	/** Get the tile position of this Prop. */
	Vector2 getWorldTilePos() const;
	/** Set the Atlas rectangle of this Prop. (which part of the image is used for drawing) */
	void setAtlasRect(Rectangle atlasRect);
	/** Get the Atlas rectangle of this Prop. */
	Rectangle getAtlasRect() const;
	/** Get the collision rectangle of this Prop, relative to the Prop itself. */
	Rectangle getCollisionRect() const;
	/** Get the collision rectangle with position relative to the World. (absolute) */
	Rectangle getWorldCollisionRect() const;
	/** Get the position of the center of the collision rectangle. */
	Vector2 getCollisionCenter() const;
	/** Whether this Prop has an Interactangle or not. */
	bool getHasInteractable() const;
	/** Set the 'hasInteractable' flag. */
	void setHasInteractable(bool value);
	/** Get a pointer to the Interactable of this Prop. */
	Interactable *getInteractable() const;
	/** Set the type of this Prop's Interactable. */
	void setInteractableType(const std::string &type);
	/** Get the type of this Prop's Interactable. */
	std::string getInteractableType() const;
	/** Draw routine. */
	void draw() const;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(atlasRect), CEREAL_NVP(collisionRect), CEREAL_NVP(imagePath), CEREAL_NVP(hasInteractable),
		  cereal::make_nvp("interactableType", getInteractableType()));
	}

	void loadFile(std::filesystem::path filePath) override {
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
		this->collisionRect =
			Rectangle{static_cast<float>(collisionRectVec[0]), static_cast<float>(collisionRectVec[1]),
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

	void saveFile(std::filesystem::path filePath) override {}
};

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(Prop)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Prop)

#endif
