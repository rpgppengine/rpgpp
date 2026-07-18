#include "widgets/propertyFields/nPatchInfoField.hpp"

#include "TGUI/SubwidgetContainer.hpp"
#include "childWindows/nPatchInfoWindow.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

NPatchInfoField::NPatchInfoField(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);

	value = tgui::Button::create();
	value->setText("NPatchInfo..");
	value->onClick([this] {
		auto *ptr = Editor::instance->getGui().getChildWindowSubService()->getWindow("edit_npatch");
		NPatchInfoWindow *window = static_cast<NPatchInfoWindow *>(ptr);

		window->view->setup(info, imageRef->texture, imageRef->scale);
		window->init(info, imageRef->texture);
		window->open();
	});

	m_container->add(label);
	m_container->add(value);

	updateSize();
}

NPatchInfoField::Ptr NPatchInfoField::create() { return std::make_shared<NPatchInfoField>(); }

NPatchInfoField::Ptr NPatchInfoField::create(const tgui::String &label, const tgui::String &value) {
	auto ptr = std::make_shared<NPatchInfoField>();
	ptr->label->setText(label);
	ptr->value->setText(value);
	return ptr;
}

NPatchInfoField::Ptr NPatchInfoField::copy(NPatchInfoField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<NPatchInfoField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr NPatchInfoField::clone() const { return std::make_shared<NPatchInfoField>(*this); }

void NPatchInfoField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void NPatchInfoField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setPosition({getSize().x * 0.5, 0});
}
