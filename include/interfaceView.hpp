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
#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "tween.hpp"
#include "tweenContainer.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;

protected:
	InterfaceViewBin bin;

	Coordinator ecs;

	std::string focusedElementName = "";
	EntityID current = MAX_ENTITIES;

	std::string scriptSource = "";
	sol::environment env;

	float f = 0;
	Tween t;
	std::list<TweenContainer> tweens = {};

public:
	bool runScript = true;

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
	void changeFocusedElement(EntityID entity);
	const std::set<EntityID> &getElements();
	void resetElements();

	virtual void onNotify(Event event);
	virtual void update();
	virtual void draw();

	Coordinator &getCoordinator();
	const std::set<EntityID> &getEntities();

	nlohmann::json dumpEntityJson(EntityID entity);
	void initEntityComponents(EntityID entity);

	void setScriptFile(const std::string &fileName);
	std::string getScriptFile();

	sol::environment &getLuaEnvironment();

	std::list<TweenContainer> &getTweens();
	void addTweenContainer(TweenContainer tweenContainer);
	void addTween(Tween tween);
};

#endif
