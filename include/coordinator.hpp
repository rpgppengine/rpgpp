#ifndef _RPGPP_COORDINATOR_H
#define _RPGPP_COORDINATOR_H

#include <memory>

#include "component.hpp"
#include "componentManager.hpp"
#include "entity.hpp"
#include "entityManager.hpp"
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
};

#endif