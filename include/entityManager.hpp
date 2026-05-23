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
	std::array<std::string, MAX_ENTITIES> names;

public:
	EntityManager();
	EntityID newEntity(const std::string &name);
	void destroyEntity(EntityID id);
	void setSignature(EntityID id, Signature signature);
	Signature getSignature(EntityID id);
	EntityID findName(const std::string &name);
	const std::string &getName(EntityID id);
};

#endif