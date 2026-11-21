#include "widgets/propertyList.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/label.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"
#include "widgets/values/valuewidget.hpp"

using namespace editor;

PropertyList::PropertyList() : edui::ChildWindow() {
	this->render->padding = 0;

	content = std::make_shared<edui::VerticalContainer>();
	content->render->padding = INTERFACE_SPACING;
	content->render->border = 0;
	content->render->as<edui::VerticalContainerRender>().space = INTERFACE_SPACING;
	content->setSize({1, 0}, {1, 0});
	add(content);
}

std::shared_ptr<ValueWidget> PropertyList::addProperty(const std::string &title, ElementProperty &prop) {
	auto container = std::make_shared<edui::Container>();
	container->render->border = 0;
	container->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	content->add(container);

	auto label = std::make_shared<edui::Label>();
	label->render->border = 0;
	label->setSize({0.5, 0}, {1, 0});
	label->setText(title);
	container->add(label);

	auto &factory = Editor::getInstance().getResources().getVariantFactory();
	auto entry = factory.getEntryVar(prop);
	auto widget = entry.instantiate();
	widget->setPosition({0.5, 0}, {0, 0});
	widget->setSize({0.5, 0}, {1, 0});
	widget->as<ValueWidget>().setValue(prop);
	container->add(widget);

	return widget;
}
