#ifndef _EDITOR_WIDGETS_VALUES_BOOLVALUE_H
#define _EDITOR_WIDGETS_VALUES_BOOLVALUE_H

#include <memory>

#include "edui/checkbox.hpp"
#include "widgets/values/valuewidget.hpp"
namespace editor {
struct BoolValue : public ValueWidgetT<bool> {
	BoolValue();

	void setValue(const bool &value) override;
	bool getValue() override;

private:
	std::shared_ptr<edui::Checkbox> checkbox;
};
}  // namespace editor

#endif
