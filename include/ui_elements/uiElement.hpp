#ifndef _RPGPP_UI_ELEMENTS_H
#define _RPGPP_UI_ELEMENTS_H

#include <functional>

#include "gamedata.hpp"

/** The maximum number of UI elements allowed in an InterfaceView. */
#define MAX_ELEMENTS (255)

/** A struct, representing an UI element in a View. */
struct UIElement {
	/** A map of the properties of this element. */
	std::unordered_map<std::string, ElementProperty> props = {};

	/** A map of the callbacks that this element has. */
	std::unordered_map<std::string, std::function<void()>> callbacks = {};

	/** Name of the type of this UI element. */
	std::string typeName = "";

	/** Empty constructor. */
	UIElement();

	/** Constructor with a name for the element type. Shall be used by derived structs. */
	UIElement(const std::string &typeName);

	/** Virtual destructor. */
	virtual ~UIElement() {};

	/** Initialize the properties and callbacks. */
	virtual void init();

	/** Routine to update the element after a property has been changed. */
	virtual void config();

	/** Update routine. */
	virtual void update() = 0;

	/** Draw routine using a rect. */
	virtual void draw(Rectangle rect) = 0;

	/** Notify routine, element responds to an event. */
	virtual void onNotify(Event event);
};

/** 8-bit number representing a number identifier for an UIElement within an InterfaceView. */
typedef uint8_t ElementIndex;

#endif
