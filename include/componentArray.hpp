#ifndef _RPGPP_COMPONENTARRAY_H
#define _RPGPP_COMPONENTARRAY_H

#include <array>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>

#include "entity.hpp"
class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(EntityID id) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
private:
	std::array<T, MAX_ENTITIES> components;
	std::unordered_map<EntityID, std::size_t> entityToIndex;
	std::unordered_map<std::size_t, EntityID> indexToEntity;
	std::size_t size;

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

	void entityDestroyed(EntityID entity) {
		if (entityToIndex.find(entity) != entityToIndex.end()) {
			removeData(entity);
		}
	}
};

#endif