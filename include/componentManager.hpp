#ifndef _RPGPP_COMPONENTMANAGER_H
#define _RPGPP_COMPONENTMANAGER_H

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "component.hpp"
#include "componentArray.hpp"
#include "entity.hpp"

class ComponentManager {
private:
	std::unordered_map<const char *, ComponentType> componentTypes;
	std::unordered_map<const char *, std::shared_ptr<IComponentArray>> arrays;
	ComponentType nextType;

	template <typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray() {
		const char *typeName = typeid(T).name();

		if (componentTypes.find(typeName) == componentTypes.end()) {
			throw std::runtime_error("Component not registered.");
		}

		return std::static_pointer_cast<ComponentArray<T>>(arrays[typeName]);
	}

public:
	ComponentManager() {
		componentTypes = {};
		arrays = {};
		nextType = {};
	}

	template <typename T>
	void registerComponent() {
		const char *typeName = typeid(T).name();

		if (componentTypes.find(typeName) != componentTypes.end()) {
			throw std::runtime_error("Component already registered.");
		}

		componentTypes.insert({typeName, nextType});

		arrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		nextType++;
	}

	template <typename T>
	ComponentType getComponentType() {
		const char *typeName = typeid(T).name();

		if (componentTypes.find(typeName) == componentTypes.end()) {
			throw std::runtime_error("Component not registered.");
		}

		return componentTypes[typeName];
	}

	template <typename T>
	void addComponent(EntityID entity, T component) {
		getComponentArray<T>()->insertData(entity, component);
	}

	template <typename T>
	void removeComponent(EntityID entity) {
		getComponentArray<T>()->removeData(entity);
	}

	template <typename T>
	T getComponent(EntityID entity) {
		return getComponentArray<T>()->getData(entity);
	}

	void entityDestroyed(EntityID entity) {
		for (const auto &pair : arrays) {
			const auto &component = pair.second;
			component->entityDestroyed(entity);
		}
	}
};

#endif