#include "widgets/propertyFields/rectangleField.hpp"

#include <climits>

#include "TGUI/Layout.hpp"
#include "TGUI/Widget.hpp"
#include "raylib.h"
#include "widgets/propertyFields/fieldConfig.hpp"

void setAlign(tgui::Label::Ptr widget) {
	widget->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	widget->setVerticalAlignment(tgui::VerticalAlignment::Center);
}

RectangleField::RectangleField(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	setAlign(label);

	l_x = tgui::Label::create("X");
	setAlign(l_x);
	value_x = tgui::SpinControl::create();
	value_x->setMinimum(INT_MIN);
	value_x->setMaximum(INT_MAX);
	value_x->onValueChange([this](const float &_) { onChange.emit(this, getValue()); });

	l_y = tgui::Label::create("Y");
	setAlign(l_y);
	value_y = tgui::SpinControl::create();
	value_y->setMinimum(INT_MIN);
	value_y->setMaximum(INT_MAX);
	value_y->onValueChange([this](const float &_) { onChange.emit(this, getValue()); });

	l_width = tgui::Label::create("W");
	setAlign(l_width);
	value_width = tgui::SpinControl::create();
	value_width->setMaximum(INT_MAX);
	value_width->onValueChange([this](const float &_) { onChange.emit(this, getValue()); });

	l_height = tgui::Label::create("H");
	setAlign(l_height);
	value_height = tgui::SpinControl::create();
	value_height->setMaximum(INT_MAX);
	value_height->onValueChange([this](const float &_) { onChange.emit(this, getValue()); });

	m_container->add(label);

	m_container->add(l_x);
	m_container->add(value_x);
	m_container->add(l_y);
	m_container->add(value_y);
	m_container->add(l_width);
	m_container->add(value_width);
	m_container->add(l_height);
	m_container->add(value_height);

	updateSize();
}

void RectangleField::setValue(Rectangle value) {
	value_x->setValue(value.x);
	value_y->setValue(value.y);
	value_width->setValue(value.width);
	value_height->setValue(value.height);
	onChange.emit(this, value);
}

Rectangle RectangleField::getValue() {
	return Rectangle{value_x->getValue(), value_y->getValue(), value_width->getValue(), value_height->getValue()};
}

RectangleField::Ptr RectangleField::create() { return std::make_shared<RectangleField>(); }

RectangleField::Ptr RectangleField::copy(RectangleField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<RectangleField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr RectangleField::clone() const { return std::make_shared<RectangleField>(*this); }

void RectangleField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void RectangleField::updateSize() {
	auto paddedW = getSize().x - PADDING * 2;
	auto height = getSize().y;
	label->setPosition({PADDING, 0});
	label->setSize({paddedW, height / 2});

	auto fieldW = paddedW / 4;
	l_x->setPosition({PADDING, tgui::bindBottom(label)});
	l_x->setSize({fieldW * 0.3, height / 2});
	value_x->setPosition({tgui::bindRight(l_x), tgui::bindBottom(label)});
	value_x->setSize({fieldW * 0.7, height / 2});
	l_y->setPosition({tgui::bindRight(value_x), tgui::bindBottom(label)});
	l_y->setSize({fieldW * 0.3, height / 2});
	value_y->setPosition({tgui::bindRight(l_y), tgui::bindBottom(label)});
	value_y->setSize({fieldW * 0.7, height / 2});
	l_width->setPosition({tgui::bindRight(value_y), tgui::bindBottom(label)});
	l_width->setSize({fieldW * 0.3, height / 2});
	value_width->setPosition({tgui::bindRight(l_width), tgui::bindBottom(label)});
	value_width->setSize({fieldW * 0.7, height / 2});
	l_height->setPosition({tgui::bindRight(value_width), tgui::bindBottom(label)});
	l_height->setSize({fieldW * 0.3, height / 2});
	value_height->setPosition({tgui::bindRight(l_height), tgui::bindBottom(label)});
	value_height->setSize({fieldW * 0.7, height / 2});
}
