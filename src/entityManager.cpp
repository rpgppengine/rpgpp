#include "entityManager.hpp"

#include <stdexcept>

#include "component.hpp"
#include "entity.hpp"

EntityManager::EntityManager() {
	availableIds = {};
	signatures = {};
	livingEntities = 0;

	for (EntityID i = 0; i < MAX_ENTITIES; i++) {
		availableIds.push(i);
	}
}

EntityID EntityManager::newEntity() {
	if (livingEntities >= MAX_ENTITIES) {
		throw std::runtime_error("Too many entities");
	}

	EntityID id = availableIds.front();
	availableIds.pop();

	livingEntities++;
	return id;
}

void EntityManager::destroyEntity(EntityID id) {
	if (id >= MAX_ENTITIES) {
		throw std::runtime_error("Out of range Entity.");
	}

	signatures[id].reset();
	availableIds.push(id);

	livingEntities--;
}

void EntityManager::setSignature(EntityID id, Signature signature) {
	if (id >= MAX_ENTITIES) {
		throw std::runtime_error("Out of range Entity.");
	}

	signatures[id] = signature;
}

Signature EntityManager::getSignature(EntityID id) {
	if (id >= MAX_ENTITIES) {
		throw std::runtime_error("Out of range Entity.");
	}

	return signatures[id];
}