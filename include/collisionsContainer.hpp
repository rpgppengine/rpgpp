#ifndef _RPGPP_COLLISIONSCONTAINER_H
#define _RPGPP_COLLISIONSCONTAINER_H

#include <raylib.h>

#include "baseContainer.hpp"
#include "basicTypes.hpp"
#include "gamedata.hpp"

/** A container of collision tiles to be used by a Room */
class CollisionsContainer : public BaseContainer<bool> {
public:
	/** Empty constructor */
	CollisionsContainer();
	void init(std::vector<IVector> &vec);
	/** Add a collision */
	void pushCollision(IVector pos);
};

#endif
