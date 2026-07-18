#ifndef _RPGPP_UI_ELEMENTS_ELEMENTFACTORY_H
#define _RPGPP_UI_ELEMENTS_ELEMENTFACTORY_H

#include <list>
#include <memory>

#include "ui_elements/uiElement.hpp"

struct ElementFactory {
	std::unordered_map<std::string, std::function<std::unique_ptr<UIElement>()>> map = {};
	std::list<std::string> elementNames = {};
	std::unique_ptr<UIElement> constructElement(const std::string &title);

	ElementFactory();
};

#endif
