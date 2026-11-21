#ifndef _EDITOR_WIDGETS_VALUES_INTVALUE_H
#define _EDITOR_WIDGETS_VALUES_INTVALUE_H

#include <memory>

#include "editor-new/widgets/values/valuewidget.hpp"
#include "edui/numberInput.hpp"

namespace editor {
struct IntValue : public ValueWidgetT<int> {
	IntValue();

	void setValue(const int &value) override;
	int getValue() override;

private:
	std::shared_ptr<edui::NumberInput> input;
};
}  // namespace editor

#endif
