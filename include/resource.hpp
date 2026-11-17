#ifndef _RESOURCE_H
#define _RESOURCE_H

#include <filesystem>

class Resource {
public:
	int typeId = 0;

	Resource(int typeId) { this->typeId = typeId; }

	virtual void afterLoad() {};
	virtual void beforeSave() {};

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}
};

enum RPGPP_RESOURCE {
	RPGPP_RESOURCE_NONE = 0,
	RPGPP_RESOURCE_TILESET,
	RPGPP_RESOURCE_ROOM,
	RPGPP_RESOURCE_ACTOR,
	RPGPP_RESOURCE_PROP,
	RPGPP_RESOURCE_INTERACTABLE,
	RPGPP_RESOURCE_UI,
	RPGPP_RESOURCE_DIALOGUE
};

#endif
