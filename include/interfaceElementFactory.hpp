#ifndef _RPGPP_INTERFACEELEMENTFACTORY_H
#define _RPGPP_INTERFACEELEMENTFACTORY_H

#include <array>
#include <functional>
#include <memory>

#include "coordinator.hpp"
#include "entity.hpp"

EntityID construct(const std::string &title, const std::string &element, Coordinator &ecs);
std::list<std::string> getElementsFactoryList();

#endif