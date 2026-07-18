#include "widgets/propertyFields/textField.hpp"

#include <TGUI/Widgets/EditBox.hpp>
#include <memory>

#include "TGUI/Texture.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

TextField::TextField(const char *typeName, bool initRenderer) : tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::EditBox::create();
	remove = tgui::BitmapButton::create();
	remove->setSize({0, 0});

	m_container->add(label);
	m_container->add(value);

	auto closeImagePath = Editor::instance->getFs().getResourcePath("close.png");
	tgui::Texture imageTexture(closeImagePath);
	remove->setImage(imageTexture);
	remove->setVisible(false);
	m_container->add(remove);

	updateSize();
}

TextField::Ptr TextField::create() { return std::make_shared<TextField>(); }

TextField::Ptr TextField::copy(TextField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<TextField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr TextField::clone() const { return std::make_shared<TextField>(*this); }

void TextField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void TextField::updateSize() {
	/*
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setPosition({getSize().x * 0.5, 0});
	*/

	if (!removable) {
		label->setPosition({PADDING, 0});
		label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
		value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
		value->setPosition({getSize().x * 0.5, 0});
		remove->setPosition({0, 0});
		remove->setSize({0, 0});
	} else {
		label->setPosition({PADDING, 0});
		label->setSize({getSize().x * 0.4f, getSize().y});
		value->setPosition({getSize().x * 0.4f, 0});
		value->setSize({getSize().x * 0.4f, getSize().y});
		remove->setPosition({getSize().x * 0.8f, 0});
		remove->setSize({getSize().x * 0.2f, getSize().y});
	}
}

void TextField::enableRemoving() {
	removable = true;
	remove->setVisible(true);
	updateSize();
}
