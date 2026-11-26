#ifndef _EDITOR_WIDGETS_PROPERTYLIST_H
#define _EDITOR_WIDGETS_PROPERTYLIST_H

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct PropertyList : public edui::ChildWindow {
	PropertyList();

	template <typename T>
	std::shared_ptr<ValueWidget> addProperty(const std::string &title, T &prop, int referId = 0) {
		int heightMultiplier = 1;

		auto container = std::make_shared<edui::Container>();
		container->render->border = 0;
		container->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
		content->add(container);
		container->referId = referId;

		auto label = std::make_shared<edui::Label>();
		label->translationId = TextFormat("%s.%s", translationId.c_str(), title.c_str());
		label->render->border = 0;
		label->setSize({0.5, 0}, {1, 0});
		label->setText(title);
		container->add(label);

		auto &factory = Editor::getInstance().getResources().getVariantFactory();
		auto entry = factory.getEntry<T>();
		auto widget = entry.instantiate();
		widget->setPosition({0.5, 0}, {0, 0});
		widget->setSize({0.5, 0}, {1, 0});
		widget->setValue(prop);
		container->add(widget);
		widget->referId = referId;

		heightMultiplier = widget->preferredSize;
		container->setSize({1, 0}, {0, heightMultiplier * edui::EDUI_SECONDARY_HEIGHT});

		return widget;
	}

	template <typename T>
	std::shared_ptr<ValueWidget> addPropertyDefault(const std::string &title, int referId) {
		T var;
		return addProperty(title, var, referId);
	}

	std::shared_ptr<ValueWidget> addPropertyVar(const std::string &title, ElementProperty &prop, int referId = 0);

	std::shared_ptr<ValueWidget> getProperty(int referId);

private:
	std::shared_ptr<edui::VerticalContainer> content;
};
}  // namespace editor

#endif
