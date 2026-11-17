#include "collisionsContainer.hpp"

#include <raylib.h>

#include "gamedata.hpp"

CollisionsContainer::CollisionsContainer() = default;

void CollisionsContainer::init(std::vector<IVector> &vec) {
	for (auto &bin : vec) {
		pushObject(bin, true);
	}
}

void CollisionsContainer::pushCollision(IVector pos) { pushObject(pos, true); }
