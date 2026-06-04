#ifndef _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H
#define _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H

#include <functional>
#include <string>
#include <unordered_map>

#include "interfaceView.hpp"
#include "rttr/property.h"
#include "rttr/variant.h"
#include "widgets/propertiesBox.hpp"

class VariantPropVisitor {
public:
	static PropertiesBox *box;
	static InterfaceView *view;
	static rttr::variant component;

	VariantPropVisitor();

	std::unordered_map<std::string, std::function<void(rttr::property)>> map;
	void componentVisit(rttr::variant component, PropertiesBox *box);

	static void Rect(std::string name, rttr::variant var);

	static void p_int(rttr::property prop);
	static void p_string(rttr::property prop);
	static void p_bool(rttr::property prop);
	static void p_Color(rttr::property prop);
	static void p_UIElementRef(rttr::property prop);
	static void p_FontRef(rttr::property prop);
	static void p_ImageRef(rttr::property prop);
	static void p_NPatchInfo(rttr::property prop);
	static void p_TextAlignment(rttr::property prop);
};

#endif