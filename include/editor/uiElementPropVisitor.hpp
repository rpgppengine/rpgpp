#ifndef _RPGPP_UIELEMENTPROPVISITOR_H
#define _RPGPP_UIELEMENTPROPVISITOR_H

#include "widgets/propertiesBox.hpp"

struct UIElementPropVisitor {
	PropertiesBox *box = nullptr;
	InterfaceView *view = nullptr;
	UIElement *element = nullptr;
	std::string key;
	void operator()(int val);
	void operator()(bool val);
	void operator()(const std::string &val);
	void operator()(Rectangle val);
	void operator()(Color val);
	void operator()(FontRef val);
	void operator()(ImageRef val);
	void operator()(UIElementRef val);
	void operator()(VerticalAlignment val);
	void operator()(HorizontalAlignment val);
	void operator()(NPatchInfo val);
	void operator()(InputC val);

	void uiElementRef(UIElementRef *ref, const std::string &key);
};

#endif
