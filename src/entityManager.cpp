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

EntityID EntityManager::newEntity(const std::string &name) {
	if (livingEntities >= MAX_ENTITIES) {
		throw std::runtime_error("Too many entities");
	}

	if (findName(name) != MAX_ENTITIES) {
		throw std::runtime_error("This name is taken.");
	}

	EntityID id = availableIds.front();
	availableIds.pop();

	names[id] = name;

	livingEntities++;
	return id;
}

void EntityManager::destroyEntity(EntityID id) {
	if (id >= MAX_ENTITIES) {
		throw std::runtime_error("Out of range Entity.");
	}

	signatures[id].reset();
	availableIds.push(id);

	names[id].erase();

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

EntityID EntityManager::findName(const std::string &name) {
	EntityID res = MAX_ENTITIES;
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (names[i] == name) {
			res = i;
		}
	}
	return res;
}

const std::string &EntityManager::getName(EntityID id) { return names[id]; }