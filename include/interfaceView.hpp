#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include <raylib.h>

#include <functional>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "gamedata.hpp"
#include "lua.hpp"
#include "saveable.hpp"
#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "tween.hpp"
#include "tweenContainer.hpp"
#include "ui_elements/uiElement.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;

protected:
	InterfaceViewBin bin;

	std::string focusedElementName = "";

	std::string scriptSource = "";
	bool scriptFlag = false;
	sol::environment env;

	std::array<std::unique_ptr<UIElement>, MAX_ELEMENTS> elements = {};
	std::array<std::string, MAX_ELEMENTS> elementNames = {};

	ElementIndex currentElement = MAX_ELEMENTS;
	std::queue<ElementIndex> availableIds = {};
	int size = 0;

	std::list<TweenContainer> tweens = {};

public:
	bool runScript = true;

	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	InterfaceView(InterfaceViewBin &bin);

	nlohmann::json dumpJson();

	const std::string &getEntityName(int index);
	ElementIndex findByName(const std::string &title);

	bool elementExists(const std::string &title);
	ElementIndex addElement(const std::string &title, const std::string &type);
	void removeElement(const std::string &title);
	UIElement *getElement(const std::string &title);
	UIElement *getElement(ElementIndex i);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);
	void changeFocusedElement(ElementIndex index);
	UIElement *cloneElement(const std::string &title, const std::string &newTitle);
	const std::array<std::unique_ptr<UIElement>, MAX_ELEMENTS> &getElements();
	void swapElements(ElementIndex a, ElementIndex b);
	void resetElements();

	ElementIndex getSize();

	virtual void onNotify(Event event);
	virtual void update();
	virtual void draw();
	virtual void drawEntity(ElementIndex i);

	void setScriptFile(const std::string &fileName);
	std::string getScriptFile();
	bool hasScript();

	sol::environment &getLuaEnvironment();

	std::list<TweenContainer> &getTweens();
	void addTweenContainer(TweenContainer tweenContainer);
	void addTween(Tween tween);

	void abandonLua();
};

#endif
