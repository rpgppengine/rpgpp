#ifndef _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H
#define _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H

#include <functional>
#include <string>
#include <unordered_map>

#include "rttr/variant.h"
#include "widgets/propertiesBox.hpp"

class VariantPropVisitor {
public:
	std::unordered_map<std::string, std::function<void(std::string, rttr::variant, PropertiesBox *)>> funcs;

	VariantPropVisitor();
	void visit(std::string name, rttr::variant var, PropertiesBox *box);

	static void Rect(std::string name, rttr::variant var, PropertiesBox *box);
};

#endif