#ifndef _RPGPP_INTERFACEELEMENTFACTORY_H
#define _RPGPP_INTERFACEELEMENTFACTORY_H

#include <array>
#include <functional>
#include <memory>

#include "coordinator.hpp"
#include "entity.hpp"
#include "uiElement.hpp"

std::unique_ptr<UIElement> constructElement(InterfaceElementType type);

EntityID construct(const std::string &title, const std::string &element, Coordinator &ecs);
std::list<std::string> getElementsFactoryList();

#endif