#include "widgets/propertyList.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/valuewidget.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"

using namespace editor;

PropertyList::PropertyList() : edui::ChildWindow() {
	content = std::make_shared<edui::VerticalContainer>();
	content->render->padding = INTERFACE_SPACING;
	content->setSize({1, 0}, {1, 0});
	add(content);
}

void PropertyList::addProperty(ElementProperty &prop) {
	auto &factory = Editor::getInstance().getResources().getVariantFactory();
	auto entry = factory.getEntryVar(prop);
	auto widget = entry.instantiate();
	widget->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	// widget->as<edui::ValueWidget>().setValue(prop);
	content->add(widget);
}
