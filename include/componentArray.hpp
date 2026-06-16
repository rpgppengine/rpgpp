#ifndef _RPGPP_COMPONENTARRAY_H
#define _RPGPP_COMPONENTARRAY_H

#include <array>
#include <cstddef>
#include <nlohmann/json.hpp>
#include <sol/object.hpp>
#include <sol/userdata.hpp>
#include <stdexcept>
#include <unordered_map>

#include "entity.hpp"
#include "jsonConversions.hpp"
#include "lua.h"
#include "nlohmann/json_fwd.hpp"
#include "rttr/variant.h"
#include "sol/forward.hpp"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(EntityID id) = 0;
	virtual rttr::variant getDataVariant(EntityID entity) = 0;
	virtual nlohmann::json getDataJson(EntityID entity) = 0;
	virtual void insertFromJson(EntityID entity, nlohmann::json json) = 0;
	virtual sol::object getLua(EntityID entity, lua_State *L) = 0;
	virtual void insertEmptyData(EntityID entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
private:
	std::array<T, MAX_ENTITIES> components;
	std::unordered_map<EntityID, std::size_t> entityToIndex;
	std::unordered_map<std::size_t, EntityID> indexToEntity;
	std::size_t size = 0;

public:
	void insertData(EntityID entity, T component) {
		if (entityToIndex.find(entity) != entityToIndex.end()) {
			throw std::runtime_error("Component added to Entity more than once.");
		}

		auto newIndex = size;

		components[newIndex] = component;
		entityToIndex[entity] = newIndex;
		indexToEntity[newIndex] = entity;
		size++;
	}

	void insertEmptyData(EntityID entity) {
		T component;
		insertData(entity, component);
	}

	void removeData(EntityID entity) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			throw std::runtime_error("Cannot remove a non-existing component.");
		}

		auto index = indexToEntity[entity];
		std::size_t lastRemovedElement = this->size - 1;
		components[index] = components[lastRemovedElement];

		EntityID lastRemovedEntity = indexToEntity[lastRemovedElement];
		entityToIndex[lastRemovedEntity] = index;
		indexToEntity[index] = lastRemovedEntity;

		entityToIndex.erase(entity);
		indexToEntity.erase(index);

		size--;
	}

	T &getData(EntityID entity) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			throw std::runtime_error("Cannot get non-existing component.");
		}

		return components[entityToIndex[entity]];
	}

	rttr::variant getDataVariant(EntityID entity) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			throw std::runtime_error("Cannot get non-existing component.");
		}

		T *component = &components[entityToIndex[entity]];
		rttr::variant variant = component;
		return variant;
	}

	nlohmann::json getDataJson(EntityID entity) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			throw std::runtime_error("Cannot get non-existing component.");
		}

		T component = components[entityToIndex[entity]];
		nlohmann::json j = component;
		return j;
	}

	void insertFromJson(EntityID entity, nlohmann::json json) {
		T component = json;
		insertData(entity, component);
	}

	sol::object getLua(EntityID entity, lua_State *L) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			throw std::runtime_error("Cannot get non-existing component.");
		}

		sol::object obj = sol::make_object(L, &components[entityToIndex[entity]]);
		return obj;
	}

	void entityDestroyed(EntityID entity) {
		if (entityToIndex.find(entity) != entityToIndex.end()) {
			removeData(entity);
		}
	}
};

#endif