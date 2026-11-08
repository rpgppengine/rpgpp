#ifndef _RPGPP_BASECONTAINER_H
#define _RPGPP_BASECONTAINER_H

#include <map>
#include <stdexcept>

#include "conversion.hpp"
#include "gamedata.hpp"
#include "raylib.h"

template <typename T>
class BaseContainer {
protected:
	/** The list of objects kept inside of this container. */
	std::map<IVector, T> objects = {};

public:
	BaseContainer<T>() {}

	/** Pushes an object to the map. */
	void pushObject(IVector pos, T obj) { this->objects.try_emplace(pos, std::move(obj)); }
	void pushObjectVec2(Vector2 pos, T obj) { pushObject(fromVector2(pos), obj); }
	/** Remove an object existing at a position. */
	void removeObject(IVector pos) {
		auto key = this->objects.find(pos);
		if (key != this->objects.end()) {
			this->objects.erase(key);
		}
	}
	void removeObjectVec2(Vector2 pos) { removeObject(fromVector2(pos)); }
	/** Check if an object exists at specified position. */
	bool objectExistsAtPosition(IVector pos) { return this->objects.find(pos) != this->objects.end(); }
	bool objectExistsAtPositionVec2(Vector2 pos) { return objectExistsAtPosition(fromVector2(pos)); }
	/** Gets the object at a specified IVector Position. */
	T &getObjectAtPosition(IVector pos) {
		if (this->objects.find(pos) != this->objects.end()) return this->objects[pos];
		throw std::out_of_range("key not found!");
	}
	/** Get a reference to the whole object map. */
	std::map<IVector, T> &getObjects() { return this->objects; }

	template <typename Archive>
	void serialize(Archive &a) {
		a(objects);
	}
};

#endif	// !_RPGPP_BASECONTAINER_H
