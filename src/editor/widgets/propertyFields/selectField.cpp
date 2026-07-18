
#include "widgets/propertyFields/selectField.hpp"

#include <memory>

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

SelectField::SelectField(const char *typeName, bool initRenderer) : tgui::Group(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::ComboBox::create();

	add(label);
	add(value);

	updateSize();
}

SelectField::Ptr SelectField::create() { return std::make_shared<SelectField>(); }

SelectField::Ptr SelectField::copy(SelectField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<SelectField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr SelectField::clone() const { return std::make_shared<SelectField>(*this); }

void SelectField::setSize(const tgui::Layout2d &size) {
	tgui::Group::setSize(size);
	updateSize();
}

void SelectField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});

	value->setPosition({getSize().x * 0.5, 0});
}
