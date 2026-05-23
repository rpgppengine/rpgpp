#ifndef _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H
#define _RPGPP_RPGPP_RTTRVARIANTPROPVISITOR_H

#include <functional>
#include <string>
#include <unordered_map>

#include "interfaceView.hpp"
#include "rttr/variant.h"
#include "widgets/propertiesBox.hpp"

class VariantPropVisitor {
public:
	static PropertiesBox *box;
	static InterfaceView *view;

	std::unordered_map<std::string, std::function<void(std::string, rttr::variant)>> funcs;

	VariantPropVisitor();
	void visit(std::string name, rttr::variant var, PropertiesBox *box);

	static void Rect(std::string name, rttr::variant var);
	static void InputComponent(std::string name, rttr::variant var);
	static void LabelComponent(std::string name, rttr::variant var);
	static void TextAreaComponent(std::string name, rttr::variant var);
	static void ColorRectComponent(std::string name, rttr::variant var);
	static void ImageRectComponent(std::string name, rttr::variant var);
	static void NinePatchImageRectComponent(std::string name, rttr::variant var);
	static void DialogueComponent(std::string name, rttr::variant var);
};

#endif