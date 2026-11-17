#ifndef _RPGPP_ACTORCONTAINER_H
#define _RPGPP_ACTORCONTAINER_H

#include <vector>

#include "actor.hpp"
#include "baseContainer.hpp"
#include "roomtypes.hpp"

class ActorContainer : public BaseContainer<ActorEntry> {
private:
	std::map<std::string, std::unique_ptr<Actor>> actors;

public:
	/** Construct the actor container. */
	ActorContainer() = default;
	void init(std::vector<ActorInRoomBin> &vec);
	/** Get an Actor with the specified name */
	Actor &getActor(const std::string &name);
	/** Add a new Actor with a name from the GameBin and an in-room name*/
	void addActor(Vector2 pos, const std::string &typeName, const std::string &roomName);
	/** Remove an Actor */
	void removeActor(const std::string &name);
	/** Check whether an Actor with such an in-room name exists. */
	bool actorExists(const std::string &name);
};

#endif	// !_RPGPP_ACTORCONTAINER_H
