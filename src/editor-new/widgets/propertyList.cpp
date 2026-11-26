#include "widgets/propertyList.hpp"

#include <memory>
#include <type_traits>
#include <variant>

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
	closeButtonPtr->visible = false;
	this->render->padding = 0;

	content = std::make_shared<edui::VerticalContainer>();
	content->render->padding = INTERFACE_SPACING;
	content->render->border = 0;
	content->render->as<edui::VerticalContainerRender>().space = INTERFACE_SPACING;
	content->setSize({1, 0}, {1, 0});
	add(content);
}

std::shared_ptr<ValueWidget> PropertyList::addPropertyVar(const std::string &title, ElementProperty &prop,
														  int referId) {
	std::shared_ptr<ValueWidget> widget;
	std::visit(
		[this, title, prop, &widget](auto &&arg) {
			using T = std::decay_t<decltype(arg)>;
			widget = addProperty(title, std::get<T>(prop));
		},
		prop);
	return widget;
	/*
		auto container = std::make_shared<edui::Container>();
		container->render->border = 0;
		container->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
		content->add(container);
		container->referId = referId;

		auto label = std::make_shared<edui::Label>();
		label->render->border = 0;
		label->translationId = TextFormat("%s.%s", translationId.c_str(), title.c_str());
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
		widget->referId = referId;

		return widget;
		*/
}

std::shared_ptr<ValueWidget> PropertyList::getProperty(int referId) {
	std::shared_ptr<ValueWidget> result = nullptr;
	auto container = content->getWidgetById(referId);
	if (container != nullptr) {
		if (container->isContainer) {
			auto valueWidget = container->as<edui::Container>().getWidgetById(referId);
			result = std::dynamic_pointer_cast<ValueWidget>(valueWidget);
		}
	}
	return result;
}
