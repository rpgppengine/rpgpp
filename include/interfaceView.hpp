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
#include "uiElement.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;
	std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> elements;

protected:
	Coordinator ecs;

	UIElement *focused = nullptr;
	std::string focusedElementName = "";
	EntityID current = MAX_ENTITIES;

public:
	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	InterfaceView(InterfaceViewBin &bin);

	nlohmann::json dumpJson();

	bool elementExists(const std::string &title);
	void addElement(const std::string &title, UIElement *element, int layer);
	void addElement(const std::string &title, std::unique_ptr<UIElement> element, int layer);
	void removeElement(const std::string &title);
	UIElement *getElement(const std::string &title);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);
	const std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> &getElements();

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
