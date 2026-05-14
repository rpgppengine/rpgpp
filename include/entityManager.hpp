#ifndef _RPGPP_ENTITYMANAGER_H
#define _RPGPP_ENTITYMANAGER_H

#include <array>
#include <cstdint>
#include <queue>

#include "component.hpp"
#include "entity.hpp"
class EntityManager {
private:
	std::queue<EntityID> availableIds;
	std::array<Signature, MAX_ENTITIES> signatures;
	uint16_t livingEntities;

public:
	EntityManager();
	EntityID newEntity();
	void destroyEntity(EntityID id);
	void setSignature(EntityID id, Signature signature);
	Signature getSignature(EntityID id);
};

#endif