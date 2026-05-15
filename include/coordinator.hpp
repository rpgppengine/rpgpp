#ifndef _RPGPP_COORDINATOR_H
#define _RPGPP_COORDINATOR_H

#include <memory>
#include <set>
#include <string>

#include "component.hpp"
#include "componentManager.hpp"
#include "entity.hpp"
#include "entityManager.hpp"
#include "rttr/variant.h"
#include "sol/forward.hpp"
#include "system.hpp"

class Coordinator {
private:
	std::unique_ptr<EntityManager> entities;
	std::unique_ptr<ComponentManager> components;
	std::unique_ptr<System> system;

public:
	Coordinator() = default;
	void init() {
		entities = std::make_unique<EntityManager>();
		components = std::make_unique<ComponentManager>();
		system = std::make_unique<System>();

		system->entitiesManager = entities.get();
		system->components = components.get();
	}

	EntityID createEntity() {
		auto entity = entities->newEntity();
		system->entities.insert(entity);
		return entity;
	}

	void destroyEntity(EntityID entity) {
		entities->destroyEntity(entity);
		system->entities.erase(entity);
	}

	template <typename T>
	void registerComponent() {
		components->registerComponent<T>();
	}

	template <typename T>
	void addComponent(EntityID entity, T component) {
		components->addComponent(entity, component);

		auto signature = entities->getSignature(entity);
		signature.set(components->getComponentType<T>(), true);
		entities->setSignature(entity, signature);
	}

	template <typename T>
	void removeComponent(EntityID entity) {
		components->removeComponent<T>(entity);

		auto signature = entities->getSignature(entity);
		signature.set(components->getComponentType<T>(), false);
		entities->setSignature(entity, signature);
	}

	template <typename T>
	bool hasComponent(EntityID entity) {
		auto signature = entities->getSignature(entity);
		return signature.test(components->getComponentType<T>());
	}

	template <typename T>
	T &getComponent(EntityID entity) {
		return components->getComponent<T>(entity);
	}

	template <typename T>
	ComponentType getComponentType() {
		return components->getComponentType<T>();
	}

	void draw() { system->draw(); }

	const std::set<EntityID> &getEntities() { return system->entities; }

	std::set<std::string> getEntityComponents(EntityID entity) const {
		std::set<std::string> set = {};
		for (int i = 0; i < MAX_COMPONENTS; i++) {
			if (entities->getSignature(entity).test(i)) {
				auto name = components->getComponentName(i);
				set.insert(name);
			}
		}
		return set;
	};

	rttr::variant getComponentVariant(EntityID entity, std::string componentName) const {
		return components->getComponentVariant(entity, componentName);
	}

	nlohmann::json getComponentJson(EntityID entity, std::string componentName) const {
		return components->getComponentJson(entity, componentName);
	}

	void insertComponentFromJson(EntityID entity, std::string componentName, nlohmann::json json) const {
		auto signature = entities->getSignature(entity);
		signature.set(components->getComponentType(componentName), true);
		entities->setSignature(entity, signature);
		components->insertComponentFromJson(entity, componentName, json);
	}

	sol::object getLuaObject(EntityID entity, std::string componentName, lua_State *L) const {
		return components->getLua(entity, componentName, L);
	}
};

#endif