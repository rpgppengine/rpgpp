#include "widgets/propertyFields/colorField.hpp"

#include <algorithm>
#include <memory>

#include "TGUI/Color.hpp"
#include "TGUI/Widgets/ColorPicker.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

ColorField::ColorField(const char *typeName, bool initRenderer) : tgui::Group(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);

	value = tgui::Button::create();
	setColor(RED);

	panel = tgui::ColorPicker::create();
	panel->setVisible(false);

	std::weak_ptr<tgui::ColorPicker> weakPanel = panel;
	value->onPress([weakPanel, this] {
		auto picker = tgui::ColorPicker::create();
		picker->setTitle("Set Color..");
		picker->setColor(color);
		picker->onOkPress([this](tgui::Color newColor) {
			setColor(newColor);
			onColorChanged.emit(this,
								{newColor.getRed(), newColor.getGreen(), newColor.getBlue(), newColor.getAlpha()});
		});
		Editor::instance->getGui().centerWidget(picker);
		Editor::instance->getGui().gui->add(picker);
	});

	add(label);
	add(value);
	add(panel);

	updateSize();
}

ColorField::Ptr ColorField::create() { return std::make_shared<ColorField>(); }

ColorField::Ptr ColorField::create(const tgui::String &label, const tgui::String &value) {
	auto ptr = std::make_shared<ColorField>();
	ptr->label->setText(label);
	ptr->value->setText(value);
	return ptr;
}

ColorField::Ptr ColorField::copy(ColorField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<ColorField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr ColorField::clone() const { return std::make_shared<ColorField>(*this); }

void ColorField::setSize(const tgui::Layout2d &size) {
	tgui::Group::setSize(size);
	updateSize();
}

void ColorField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setPosition({getSize().x * 0.5, 0});
}

void ColorField::setColor(Color color) {
	tgui::Color newColor{color.r, color.g, color.b, color.a};
	tgui::Color hoverColor{static_cast<uint8_t>(clamp(color.r + 20, 0, 255)),
						   static_cast<uint8_t>(clamp(color.g + 20, 0, 255)),
						   static_cast<uint8_t>(clamp(color.b + 20, 0, 255)), color.a};

	value->getRenderer()->setBackgroundColor(newColor);
	value->getRenderer()->setBackgroundColorHover(hoverColor);
	value->getRenderer()->setBackgroundColorDown(hoverColor);

	this->color = newColor;
}

void ColorField::setColor(tgui::Color color) {
	tgui::Color newColor = color;
	tgui::Color hoverColor{static_cast<uint8_t>(clamp(color.getRed() + 20, 0, 255)),
						   static_cast<uint8_t>(clamp(color.getGreen() + 20, 0, 255)),
						   static_cast<uint8_t>(clamp(color.getBlue() + 20, 0, 255)), color.getAlpha()};

	value->getRenderer()->setBackgroundColor(newColor);
	value->getRenderer()->setBackgroundColorHover(hoverColor);
	value->getRenderer()->setBackgroundColorDown(hoverColor);

	this->color = color;
}