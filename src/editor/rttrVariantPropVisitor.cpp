#include "rttrVariantPropVisitor.hpp"

#include "widgets/propertyFields/rectangleField.hpp"

VariantPropVisitor::VariantPropVisitor() { funcs["Rectangle"] = Rect; }

void VariantPropVisitor::visit(std::string name, rttr::variant var, PropertiesBox *box) {
	if (funcs[name] != nullptr) {
		funcs[name](name, var, box);
	}
}

void VariantPropVisitor::Rect(std::string name, rttr::variant var, PropertiesBox *box) {
	auto field = RectangleField::create();
	field->label->setText(name);
	field->setValue(var.get_wrapped_value<Rectangle>());
	box->addRectangleField(field);
}