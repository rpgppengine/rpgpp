#ifndef _RPGPP_SYSTEM_H
#define _RPGPP_SYSTEM_H

#include <set>

#include "componentManager.hpp"
#include "entity.hpp"
#include "entityManager.hpp"

class System {
private:
	template <typename T>
	bool hasComponent(EntityID entity) {
		auto signature = entitiesManager->getSignature(entity);
		return signature.test(components->getComponentType<T>());
	}

public:
	ComponentManager *components = nullptr;
	EntityManager *entitiesManager = nullptr;

	std::set<EntityID> entities;
	System() = default;
	void update();
	void draw();
};

#endif