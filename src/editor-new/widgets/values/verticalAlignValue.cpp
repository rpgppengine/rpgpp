#include "widgets/values/verticalAlignValue.hpp"

#include <memory>

#include "basicTypes.hpp"
#include "edui/dropdown.hpp"

using namespace editor;

VerticalAlignValue::VerticalAlignValue() {
	dropdown = std::make_shared<edui::Dropdown>();
	dropdown->setSize({1, 0}, {1, 0});

	dropdown->addItem("top");
	dropdown->addItem("middle");
	dropdown->addItem("bottom");

	dropdown->onValueChanged.connect([this](edui::DropdownValue a, edui::DropdownValue b) {
		VerticalAlignment old = {a.idx};
		VerticalAlignment newval = {b.idx};

		onValueChanged.invoke(old, newval);
	});

	add(dropdown);
}

void VerticalAlignValue::setValue(const VerticalAlignment &value) { dropdown->setValue({value.val}); }

VerticalAlignment VerticalAlignValue::getValue() { return {dropdown->getValue().idx}; }
