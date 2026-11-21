#include "widgets/values/horizontalAlignValue.hpp"

#include <memory>

#include "basicTypes.hpp"
#include "edui/dropdown.hpp"

using namespace editor;

HorizontalAlignValue::HorizontalAlignValue() {
	dropdown = std::make_shared<edui::Dropdown>();
	dropdown->setSize({1, 0}, {1, 0});

	dropdown->addItem("left");
	dropdown->addItem("center");
	dropdown->addItem("right");

	dropdown->onValueChanged.connect([this](edui::DropdownValue a, edui::DropdownValue b) {
		HorizontalAlignment old = {a.idx};
		HorizontalAlignment newval = {b.idx};

		onValueChanged.invoke(old, newval);
	});

	add(dropdown);
}

void HorizontalAlignValue::setValue(const HorizontalAlignment &value) { dropdown->setValue({value.val}); }

HorizontalAlignment HorizontalAlignValue::getValue() { return {dropdown->getValue().idx}; }
