#ifndef _RPGPP_ANYPROPERTYVISITOR_H
#define _RPGPP_ANYPROPERTYVISITOR_H

#include "widgets/propertiesBox.hpp"

class AnyPropertyVisitor {
	static std::unordered_map<std::type_index,
							  std::function<void(PropertiesBox *, const std::string &, xxx::any_ptr &)>>
		anyPropsFuncs;

public:
	AnyPropertyVisitor();
	static void addAnyProps(PropertiesBox *propsBox, std::map<std::string, xxx::any_ptr> props);
};

#endif