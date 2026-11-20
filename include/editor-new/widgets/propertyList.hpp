#ifndef _EDITOR_WIDGETS_PROPERTYLIST_H
#define _EDITOR_WIDGETS_PROPERTYLIST_H

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/valuewidget.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"

namespace editor {
struct PropertyList : public edui::ChildWindow {
	PropertyList();

	template <typename T>
	void addProperty(T &prop) {
		auto &factory = Editor::getInstance().getResources().getVariantFactory();
		auto entry = factory.getEntry<T>();
		std::shared_ptr<edui::Widget> widget = entry.instantiate();
		widget->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
		// widget->as<edui::ValueWidget>().setValue(prop);
		content->add(widget);
	}

	void addProperty(ElementProperty &prop);

private:
	std::shared_ptr<edui::VerticalContainer> content;
};
}  // namespace editor

#endif
