#ifndef _EDITOR_WIDGETS_FILEPANELS_PROPERTYFILEPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_PROPERTYFILEPANEL_H

#include "commands/propSet.hpp"
#include "widgets/filePanels/filePanel.hpp"
#include "widgets/propertyList.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct PropertyWidgetInfo {
	ValueWidget &widget;
};

class PropertyFilePanel : public FilePanel {
public:
	PropertyFilePanel() {}

protected:
	std::shared_ptr<PropertyList> properties;

	template <typename TResource, typename T, typename TGetter, typename TSetter>
	PropertyWidgetInfo bindProp(const std::string &title, int referId, TGetter &&getter, TSetter &&setter) {
		auto widget = properties->addPropertyDefault<T>(title, referId);
		widget->template as<ValueWidgetT<T>>().onValueChanged.connect(
			[this, getter = std::forward<TGetter>(getter), setter = std::forward<TSetter>(setter)](T, T newVal) {
				auto cmd = std::make_unique<PropSet<T>>();
				auto &restype = resource->as<TResource>();
				cmd->newValue = newVal;
				cmd->getter = [&restype, &getter] { return (restype.*getter)(); };
				cmd->setter = [&restype, &setter](T val) { (restype.*setter)(val); };

				this->exec(std::move(cmd));
			});

		PropertyWidgetInfo info{widget->template as<ValueWidget>()};
		return info;
	};
};
}  // namespace editor

#endif
