#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include <raylib.h>

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "coordinator.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "lua.h"
#include "saveable.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;

protected:
	Coordinator ecs;

	std::string focusedElementName = "";
	EntityID current = MAX_ENTITIES;

public:
	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	InterfaceView(InterfaceViewBin &bin);

	nlohmann::json dumpJson();

	bool elementExists(const std::string &title);
	EntityID addElement(const std::string &title);
	void removeElement(const std::string &title);
	EntityID getElement(const std::string &title);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);
	const std::set<EntityID> &getElements();

	virtual void onNotify(Event event);
	virtual void update();
	virtual void draw();

	Coordinator &getCoordinator();
	const std::set<EntityID> &getEntities();

	void registerLua(lua_State *L);
	void changeFocusedElement(EntityID entity);
	nlohmann::json dumpEntityJson(EntityID entity);
	void initEntityComponents(EntityID entity);
};

#endif
