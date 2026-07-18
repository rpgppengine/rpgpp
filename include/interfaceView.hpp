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

/** An InterfaceView represents a collection of UI elements drawn on the screen to make up a user interface. */
class InterfaceView : public ISaveable {
private:
	Rectangle rect;

protected:
	/** The structure which was used to construct this InterfaceView. */
	InterfaceViewBin bin;

	/** The title of the currently focused element. */
	std::string focusedElementName = "";

	/** The source of the Lua script. */
	std::string scriptSource = "";
	/** Whether this view has a script or not. */
	bool scriptFlag = false;
	/** A Lua environment for this view. */
	sol::environment env;

	/** The array of UI elements. */
	std::array<std::unique_ptr<UIElement>, MAX_ELEMENTS> elements = {};
	/** The array of the elements' names. */
	std::array<std::string, MAX_ELEMENTS> elementNames = {};

	/** Index of the currently focused element. */
	ElementIndex currentElement = MAX_ELEMENTS;
	/** Queue of available element indexes. */
	std::queue<ElementIndex> availableIds = {};
	/** Amount of elements in this View. */
	int size = 0;

	/** List of the tweens currently being ran. */
	std::list<TweenContainer> tweens = {};

public:
	/** Flag whether the script should be ran. */
	bool runScript = true;

	/** Default constructor. */
	InterfaceView();
	/** Construct from a UI View file. */
	InterfaceView(const std::string &filePath);
	/** Construct from binary data. */
	InterfaceView(InterfaceViewBin &bin);

	/** Dump json data. */
	nlohmann::json dumpJson();

	/** Get name of an element by index. */
	const std::string &getEntityName(int index);
	/** Get the index of an element identified by its name. */
	ElementIndex findByName(const std::string &title);

	/** Whether an element with such a name exists. */
	bool elementExists(const std::string &title);
	/** Add an element with a title and a registered type. */
	ElementIndex addElement(const std::string &title, const std::string &type);
	/** Remove an element by its title. */
	void removeElement(const std::string &title);
	/** Get an element pointer by its title. */
	UIElement *getElement(const std::string &title);
	/** Get an element pointer by its index. */
	UIElement *getElement(ElementIndex i);
	/** Rename an element. */
	void renameElement(const std::string &title, const std::string &newTitle);
	/** Change the focus to an element identified by its index. */
	void changeFocusedElement(const std::string &title);
	/** Change the focus to an element identified by an index. */
	void changeFocusedElement(ElementIndex index);
	/** Clone an element by giving it a title. It will appear right above the element that was cloned. */
	UIElement *cloneElement(const std::string &title, const std::string &newTitle);
	/** Get an array of the elements. */
	const std::array<std::unique_ptr<UIElement>, MAX_ELEMENTS> &getElements();
	/** Swap two elements. */
	void swapElements(ElementIndex a, ElementIndex b);
	/** Move an element to another position within the array. */
	void moveToPosition(ElementIndex source, ElementIndex dest);
	/** Reset the elements' positions and sizes. */
	void resetElements();

	/** Get the amount of elements within this View. */
	ElementIndex getSize();

	/** Notify routine, the View shall react to this event. */
	virtual void onNotify(Event event);
	/** Update routine. */
	virtual void update();
	/** Draw routine. Draws all the elements. */
	virtual void draw();
	/** Draw a specific element. */
	virtual void drawEntity(ElementIndex i);

	/** Set the script file for this UI View. */
	void setScriptFile(const std::string &fileName);
	/** Get the path to the script file for this UI View. */
	std::string getScriptFile();
	/** Check whether this View has an attached script or not. */
	bool hasScript();

	/** Get a reference to the Lua environment of this View. */
	sol::environment &getLuaEnvironment();

	/** Get a reference to the list of running tweens. */
	std::list<TweenContainer> &getTweens();
	/** Add a tween container to be animated. */
	void addTweenContainer(TweenContainer tweenContainer);
	/** Add a singular tween to be animated. */
	void addTween(Tween tween);

	/** Abandon the Lua environment. Should always be done before the end of the program. */
	void abandonLua();
};

#endif
