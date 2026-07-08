#ifndef _RPGPP_UI_ELEMENTS_H
#define _RPGPP_UI_ELEMENTS_H

#include <functional>
#include "gamedata.hpp"

#define MAX_ELEMENTS (255)

struct UIElement {
	std::unordered_map<std::string, ElementProperty> props = {};
	std::array<std::function<void()>, RPGPP_CALLBACK_MAX> callbacks = {};
	std::string typeName = "";
	UIElement();
	UIElement(const std::string& typeName);
	virtual ~UIElement() {};
	virtual void init();
	virtual void config();
	virtual void update() = 0;
	virtual void draw(Rectangle rect) = 0;
	virtual void onNotify(Event event);

	void fromBin(UIElement& bin);
};

typedef uint8_t ElementIndex;

#endif
