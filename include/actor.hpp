#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include <raylib.h>

#include <array>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "animation.hpp"
#include "atlasTile.hpp"
#include "basicTypes.hpp"
#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/vector.hpp"
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

const std::string ANIM_DOWN_IDLE = "DownIdle";
const std::string ANIM_DOWN = "Down";
const std::string ANIM_UP_IDLE = "UpIdle";
const std::string ANIM_UP = "Up";
const std::string ANIM_LEFT_IDLE = "LeftIdle";
const std::string ANIM_LEFT = "Left";
const std::string ANIM_RIGHT_IDLE = "RightIdle";
const std::string ANIM_RIGHT = "Right";

const std::map<std::string, Direction> dirMap = {{ANIM_DOWN_IDLE, RPGPP_DOWN_IDLE},	  {ANIM_DOWN, RPGPP_DOWN},
												 {ANIM_UP_IDLE, RPGPP_UP_IDLE},		  {ANIM_UP, RPGPP_UP},
												 {ANIM_LEFT_IDLE, RPGPP_LEFT_IDLE},	  {ANIM_LEFT, RPGPP_LEFT},
												 {ANIM_RIGHT_IDLE, RPGPP_RIGHT_IDLE}, {ANIM_RIGHT, RPGPP_RIGHT}};

/** The Actor class represents an Actor in the game's world.
 * @see [Direction](Direction.md)
 */
class Actor : public Resource {
private:
	std::string sourcePath;
	/** The used TileSet for this Actor's sprites. */
	TileSet tileSet;
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
	bool isPlaying = true;
	/** Rectangle, representing the collision of this Actor. */
	Rectangle collisionRect = {0, 0, 0, 0};
	/** An array of the animations. Each std::vector<Vector2> represents the
	 * frames of an animation. */
	std::map<std::string, Animation> animations;
	/** A Direction enum, showing the current animation that is being played. */
	std::string currentAnimation = ANIM_DOWN_IDLE;
	std::string lastAnimation = ANIM_DOWN_IDLE;
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
	/** Unload routine. The UnloadTexture function will called here. */
	void unload() const;
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw() const;
	void setIsPlaying(bool value);
	bool getIsPlaying();
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
	TileSet &getTileSet();
	/* Get the path to the used TileSet. */
	TileSetRef getTileSetSource();
	/* Set a new TileSet using a path. */
	void setTileSetSource(const TileSetRef &source);
	/** Get the current frame of this actor. */
	int getCurrentFrame() const;
	/** Reset the animation back to frame 0. */
	void resetAnimation() { this->setCurrentFrame(0); }
	/** Get the current direction of this actor. */
	Direction getAnimationDirection() const;
	/** Get the current animation atlas of this actor. */
	IVector getCurrentAnimationAtlas() const;
	/** Get the current animation rectangle of this actor. */
	Rectangle getCurrentAnimationRectangle();
	/** Get an atlas of this actor by frame index. */
	IVector getAnimationAtlasByIdx(int frameIndex) const;
	/** Set the current frame. */
	void setCurrentFrame(int frameIndex);
	/** Get the current size of the animation in the direction. */
	int getAnimationCount() const;
	/** Set this Actor's TileSet using a path to the tileset file. */
	void setTileSet(const std::string &newTileSetSource);
	/** The callback when the current frame changes. */
	std::function<void(int)> onFrameChanged;
	/** Get the collision rectangle of this Actor if it was moved by the
	 * velocity vector */
	Rectangle getCollisionRectWithVelocity(Vector2 velocity) const;
	/** Get the collision Rectangle of this Actor */
	Rectangle getCollisionRect() const;
	/** Set the Actor's collision Rectangle */
	void setCollisionRect(const Rectangle &rect);
	/** Get collision center point. */
	Vector2 getCollisionCenter() const;
	/** Temporarily play an animation */
	void playAnimation(Direction id);
	/** Check whether a temporary animation is playing */
	bool isTempAnimationPlaying();
	/** Change the Actor's current animation and play it. */
	void changeAnimationDir(Direction id);
	void changeAnimation(const std::string &id);
	/* Get the id of the current animation. */
	std::string getCurrentAnimationId();
	/** Get the path of the used TileSet. */
	// std::string getTileSetSource() const;
	/** Get an array of this Actor's animations */
	std::map<std::string, Animation> &getAnimationsRaw();
	/** Get a specific animation */
	Animation getAnimationRaw(const std::string &id) const;
	Animation &getAnimationRef(const std::string &id);
	void addAnimation(const std::string &id, std::vector<IVector> frames);
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

	void afterLoad() override;
	void loadFile() override;
};

struct ActorEntry {
	std::string name;
	Actor actor;
};

Vector2 calcActorTilePos(Vector2 newPosition, Vector2 worldTileSize, TileSet *tileSet);

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(Actor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Actor)

#endif
