#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include <raylib.h>

#include <array>
#include <fstream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "atlasTile.hpp"
#include "basicTypes.hpp"
#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "resource.hpp"
#include "saveable.hpp"
#include "tileset.hpp"

using json = nlohmann::json;

#define RPGPP_MAX_DIRECTION 7
/** Direction enum, representing an animation state. */
enum Direction : short {
	RPGPP_DOWN_IDLE = 0,   ///< Down Idle state.
	RPGPP_DOWN = 1,		   ///< Down state.
	RPGPP_UP_IDLE = 2,	   ///< Up Idle state.
	RPGPP_UP = 3,		   ///< Up state.
	RPGPP_LEFT_IDLE = 4,   ///< Left Idle state.
	RPGPP_LEFT = 5,		   ///< Left state.
	RPGPP_RIGHT_IDLE = 6,  ///< Right Idle state.
	RPGPP_RIGHT = 7		   ///< Right state.
};

/** The Actor class represents an Actor in the game's world.
 * @see [Direction](Direction.md)
 */
class Actor : public Resource {
private:
	std::string sourcePath;
	/** The used TileSet for this Actor's sprites. */
	std::unique_ptr<TileSet> tileSet;
	/** The path to the TileSet file. */
	std::string tileSetSource;
	/** The currently shown tile from the TileSet. */
	AtlasTile tile;
	/** Current position in the world of this Actor. */
	Vector2 position = {0, 0};
	/** Current tile position of this Actor if such is set. */
	Vector2 tilePosition = {-1, -1};
	int frameCounter = 0;
	int frameSpeed = 2;
	int currentFrame = 0;
	/** Rectangle, representing the collision of this Actor. */
	Rectangle collisionRect = {0, 0, 0, 0};
	/** An array of the animations. Each std::vector<Vector2> represents the
	 * frames of an animation. */
	std::array<std::vector<IVector>, 8> animations;
	/** A Direction enum, showing the current animation that is being played. */
	Direction currentAnimation = RPGPP_DOWN_IDLE;
	Direction lastAnimation = RPGPP_DOWN_IDLE;
	bool tempAnimIsPlayed = false;
	/** Whether this Actor has an Interactable. */
	bool ownsInteractable = false;
	/** A smart pointer, owning an Interactable. */
	std::unique_ptr<Interactable> interactable = nullptr;

	friend class cereal::access;

public:
	/** Default constructor. */
	Actor();
	/** Constructor that takes a path to the .ractor file. */
	Actor(const std::string &fileName);
	/** Constructor that takes a TileSet, the atlas position of the tile to use,
	 * and the path to the TileSet. */
	Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos, std::string tileSetSource);
	/** Constructor that takes an ActorBin binary structure */
	Actor(const ActorBin &bin);
	/** Dump this Actor's data to a nlohmann::json object. */
	// json dumpJson() override;
	/** Unload routine. The UnloadTexture function will called here. */
	void unload() const;
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw() const;
	/** Get the source file's path for this Actor. */
	std::string getSourcePath() const;
	/** Get this actor's position in the world. */
	Vector2 getPosition() const;
	/** Set this Actor's position. */
	void setPosition(Vector2 newPosition);
	/** Set Actor position relative to a tile in the world. */
	void setTilePosition(Vector2 newPosition, Vector2 tileSize);
	/** Get this Actor's tile position. */
	Vector2 getTilePosition() const;
	/** Move the Actor using a velocity vector. */
	void moveByVelocity(Vector2 velocity);
	/** Get the bounding box of this Actor. */
	Rectangle getRect() const;
	/** Get a reference to this Actor's TileSet. */
	TileSet &getTileSet() const;
	/** Get the current frame of this actor. */
	int getCurrentFrame() const;
	/** Reset the animation back to frame 0. */
	void resetAnimation() { this->setAnimationFrame(0); }
	/** Get the current direction of this actor. */
	Direction getAnimationDirection() const;
	/** Get the current animation atlas of this actor. */
	IVector getCurrentAnimationAtlas() const;
	/** Get the current animation rectangle of this actor. */
	Rectangle getCurrentAnimationRectangle() const;
	/** Get an atlas of this actor by frame index. */
	IVector getAnimationAtlasByIdx(int frameIndex) const;
	/** Set the current frame. */
	void setAnimationFrame(int frameIndex);
	/** Get the current size of the animation in the direction. */
	int getAnimationCount() const;
	/** Set this Actor's TileSet using a path to the tileset file. */
	void setTileSet(const std::string &newTileSetSource);
	/** The callback when the current frame changes. */
	std::function<void(int)> onFrameChanged;
	/** Get the collision rectangle of this Actor if it was moved by the
	 * velocity vector */
	Rectangle getCollisionRect(Vector2 velocity) const;
	/** Get the collision Rectangle of this Actor */
	Rectangle getCollisionRect() const;
	/** Get collision center point. */
	Vector2 getCollisionCenter() const;
	/** Add a frame in the chosen animation. The frame represents an atlas tile
	 * from this Actor's TileSet. */
	void addAnimationFrame(Direction id, Vector2 atlasPos);
	/** Remove a frame in the chosen animation, specified by an index. */
	void removeAnimationFrame(Direction id, int frameIndex);
	/** Change a frame in the chosen animation to another atlas tile from the
	 * TileSet. */
	void setAnimationFrame(Direction id, int frameIndex, Vector2 atlasTile);
	/** Add multiple frames to the chosen animation. */
	void addAnimationFrames(Direction id, const std::vector<std::vector<int>> &frames);
	/** Temporarily play an animation */
	void playAnimation(Direction id);
	/** Check whether a temporary animation is playing */
	bool isTempAnimationPlaying();
	/** Change the Actor's current animation and play it. */
	void changeAnimation(Direction id);
	/** Get the path of the used TileSet. */
	std::string getTileSetSource() const;
	/** Get an array of this Actor's animations */
	std::array<std::vector<IVector>, 8> getAnimationsRaw() const;
	/** Get a specific animation */
	std::vector<IVector> getAnimationRaw(Direction id) const;
	/** Set the Actor's collision Rectangle */
	void setCollisionRect(Rectangle rect);
	/** Whether this Actor has an Interactable. */
	bool hasInteractable();
	/** Set the 'ownsInteractable' flag. */
	void setHasInteractable(bool value);
	/** Get a pointer to this Actor's Interactable. */
	Interactable *getInteractable();
	/** Add an Interactable using an interactable file. */
	void setInteractableFromPath(const std::string &interPath);

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(sourcePath), CEREAL_NVP(tileSetSource), CEREAL_NVP(animations), CEREAL_NVP(collisionRect));
	}

	void loadFile(std::filesystem::path filePath) override;
	void saveFile(std::filesystem::path filePath) override {
		/*
		std::ofstream stream(filePath);
		cereal::JSONOutputArchive ar(stream);
		ar(*this);
		*/
	}
};

Vector2 calcActorTilePos(Vector2 newPosition, Vector2 worldTileSize, TileSet *tileSet);

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(Actor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Actor)

#endif
