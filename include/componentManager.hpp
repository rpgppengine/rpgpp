#ifndef _RPGPP_COMPONENTMANAGER_H
#define _RPGPP_COMPONENTMANAGER_H

#include <rttr/type.h>

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "component.hpp"
#include "componentArray.hpp"
#include "entity.hpp"
#include "lua.h"
#include "rttr/variant.h"
#include "sol/forward.hpp"

class ComponentManager {
private:
	std::unordered_map<std::string, ComponentType> componentTypes;
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> arrays;
	std::unordered_map<ComponentType, std::string> componentNames;
	ComponentType nextType;

	template <typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray() {
		auto typeName = rttr::type::get<T>().get_name().to_string();

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
		auto typeName = rttr::type::get<T>().get_name().to_string();

		if (componentTypes.find(typeName) != componentTypes.end()) {
			throw std::runtime_error("Component already registered.");
		}

		componentTypes.insert({typeName, nextType});
		componentNames.insert({nextType, typeName});

		arrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		nextType++;
	}

	template <typename T>
	ComponentType getComponentType() {
		auto typeName = rttr::type::get<T>().get_name().to_string();

		if (componentTypes.find(typeName) == componentTypes.end()) {
			throw std::runtime_error("Component not registered.");
		}

		return componentTypes[typeName];
	}

	ComponentType getComponentType(const std::string &typeName) {
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
	T &getComponent(EntityID entity) {
		return getComponentArray<T>()->getData(entity);
	}

	rttr::variant getComponentVariant(EntityID entity, std::string componentName) {
		return arrays[componentName]->getDataVariant(entity);
	}

	nlohmann::json getComponentJson(EntityID entity, std::string componentName) {
		return arrays[componentName]->getDataJson(entity);
	}

	void insertComponentFromJson(EntityID entity, std::string componentName, nlohmann::json json) {
		arrays[componentName]->insertFromJson(entity, json);
	}

	void replaceComponentFromJson(EntityID entity, std::string componentName, nlohmann::json json) {
		arrays[componentName]->replaceFromJson(entity, json);
	}

	sol::object getLua(EntityID entity, std::string componentName, lua_State *L) {
		return arrays[componentName]->getLua(entity, L);
	}

	void insertEmptyData(EntityID entity, const std::string &componentName) {
		arrays[componentName]->insertEmptyData(entity);
	}

	const std::string &getComponentName(ComponentType type) { return componentNames[type]; }

	void entityDestroyed(EntityID entity) {
		for (const auto &pair : arrays) {
			const auto &component = pair.second;
			component->entityDestroyed(entity);
		}
	}

	bool componentNameExists(const std::string& name) {
		return componentTypes.count(name) > 0;
	}
};

#endif
