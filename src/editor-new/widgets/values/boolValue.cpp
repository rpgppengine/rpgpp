#include "widgets/values/boolValue.hpp"

#include <memory>

#include "edui/checkbox.hpp"

using namespace editor;

BoolValue::BoolValue() {
	checkbox = std::make_shared<edui::Checkbox>();
	checkbox->setSize({1, 0}, {1, 0});
	checkbox->onValueChanged.connect([this](bool a, bool b) { onValueChanged.invoke(a, b); });
	add(checkbox);
}

void BoolValue::setValue(const bool &value) { checkbox->setValue(value); }

bool BoolValue::getValue() { return checkbox->getValue(); }
