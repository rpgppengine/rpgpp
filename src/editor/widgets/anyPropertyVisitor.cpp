#include "widgets/anyPropertyVisitor.hpp"

#include <any>
#include <array>
#include <string>
#include <typeindex>

#include "TGUI/String.hpp"
#include "any_ptr/any_ptr.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/textField.hpp"

template <class T, class F>
inline std::pair<const std::type_index, std::function<void(PropertiesBox *, const std::string &, xxx::any_ptr &)>>
to_any_visitor(const F &f) {
	return {std::type_index(typeid(T)), [g = f](PropertiesBox *props, const std::string &title, xxx::any_ptr &a) {
				if constexpr (std::is_void_v<T>)
					g();
				else
					g(props, title, xxx::any_ptr_cast<T>(a));
			}};
}

std::unordered_map<std::type_index, std::function<void(PropertiesBox *, const std::string &, xxx::any_ptr &)>>
	AnyPropertyVisitor::anyPropsFuncs = {
		to_any_visitor<int>([](PropertiesBox *box, const std::string &title, int *val) {
			auto field = IntField::create();
			field->label->setText(title);
			field->value->setValue(*val);
			field->value->onValueChange([val](int value) { *val = value; });
			box->addIntField(field);
		}),
		to_any_visitor<std::string>([](PropertiesBox *box, const std::string &title, std::string *val) {
			auto field = TextField::create();
			field->label->setText(title);
			field->value->setText(*val);
			field->value->onTextChange([val](const tgui::String &text) { *val = text.toStdString(); });
			box->addTextField(field);
		}),
};

AnyPropertyVisitor::AnyPropertyVisitor() {}

void AnyPropertyVisitor::addAnyProps(PropertiesBox *propsBox, std::map<std::string, xxx::any_ptr> props) {
	propsBox->anyProps = props;

	for (auto &[name, ptr] : props) {
		if (anyPropsFuncs.count(std::type_index(ptr.type())) > 0) {
			anyPropsFuncs[std::type_index(ptr.type())](propsBox, name, ptr);
		}
	}
}
