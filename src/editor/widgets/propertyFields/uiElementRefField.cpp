#include "widgets/propertyFields/uiElementRefField.hpp"

#include "TGUI/SubwidgetContainer.hpp"
#include "childWindows/uiElementRefWindow.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

UIElementRefField::UIElementRefField(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);

	value = tgui::Button::create();
	value->onClick([this] {
		auto *ptr = Editor::instance->getGui().getChildWindowSubService()->getWindow("element_ref");
		UIElementRefWindow *window = static_cast<UIElementRefWindow *>(ptr);
		window->view = view;
		window->ref = ref;
		window->field = this;

		window->init();
		window->open();
	});

	m_container->add(label);
	m_container->add(value);

	updateSize();
}

UIElementRefField::Ptr UIElementRefField::create() { return std::make_shared<UIElementRefField>(); }

UIElementRefField::Ptr UIElementRefField::create(const tgui::String &label, const tgui::String &value) {
	auto ptr = std::make_shared<UIElementRefField>();
	ptr->label->setText(label);
	ptr->value->setText(value);
	return ptr;
}

UIElementRefField::Ptr UIElementRefField::copy(UIElementRefField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<UIElementRefField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr UIElementRefField::clone() const { return std::make_shared<UIElementRefField>(*this); }

void UIElementRefField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void UIElementRefField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setPosition({getSize().x * 0.5, 0});
}