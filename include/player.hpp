#ifndef _RPGPP_PLAYER_H
#define _RPGPP_PLAYER_H

class Room;

#include <raylib.h>

#include <string>

#include "actor.hpp"
#include "room.hpp"

/** The Player class represents the player character that the player controls.
 */
class Player {
private:
	bool lock;
	/** The Actor that this Player controls. */
	std::unique_ptr<Actor> actor = nullptr;
	/** The absolute position of this Player. */
	Vector2 position;
	float size;
	/** The change in the player's position. */
	Vector2 velocity;
	/** Current direction state of the Player. */
	Direction currentDirection;
	Direction idleDirection;
	Rectangle interactableArea;
	/** Handle the collision for this Player. */
	void handleCollision();
	/** Handle the interactions for this Player. */
	void handleInteraction();

public:
	Player();
	/** Construct from an Actor and the Room where the Player shall be
	 * contained. */
	Player(std::unique_ptr<Actor> actor);
	void setFromBin(const std::string &title);
	/** Unload routine. */
	void unload() const;
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw() const;
	/** Set the Player's current room. */
	void setRoom(Room &room) const;
	/** Get the player's actor */
	Actor &getActor() const;
	/** Move the player by a certain velocity. */
	void moveByVelocity(Vector2 velocity);
	/** Get the player's position. */
	Vector2 getPosition() const;
	/** Set the player's position. */
	void setPosition(Vector2 pos);
	/** Get the player's position with an 'anchor' in the center. */
	Vector2 getCenterPosition() const;
	/** Get the player's tile position. */
	Vector2 getTilePosition() const;
	/** Set the player's tile position. */
	void setTilePosition(Vector2 tilePos);
	/** Get the position of the collision. */
	Vector2 getCollisionPos() const;
	/** Get the center position of the player's collision. */
	Vector2 getCollisionCenterPos() const;
	/** Set whether the Player should be able to move or not. */
	void setMovementLock(bool value);
};

#endif
