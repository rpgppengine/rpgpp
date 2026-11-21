#ifndef _EDITOR_WIDGETS_PROPERTYLIST_H
#define _EDITOR_WIDGETS_PROPERTYLIST_H

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct PropertyList : public edui::ChildWindow {
	PropertyList();

	template <typename T>
	void addProperty(const std::string &title, T &prop) {
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
		auto entry = factory.getEntry<T>();
		std::shared_ptr<edui::Widget> widget = entry.instantiate();
		widget->setSize({0.5, 0}, {1, 0});
		widget->as<ValueWidget>().setValue(prop);
		container->add(widget);
	}

	std::shared_ptr<ValueWidget> addProperty(const std::string &title, ElementProperty &prop);

private:
	std::shared_ptr<edui::VerticalContainer> content;
};
}  // namespace editor

#endif
