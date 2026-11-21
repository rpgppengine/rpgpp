#include "widgets/values/intValue.hpp"

#include <memory>

#include "edui/numberInput.hpp"

using namespace editor;

IntValue::IntValue() {
	input = std::make_shared<edui::NumberInput>();
	input->setSize({1, 0}, {1, 0});
	input->onValueChanged.connect([this](int a, int b) { onValueChanged.invoke(a, b); });
	add(input);
}

void IntValue::setValue(const int &value) { input->setValue(value); }

int IntValue::getValue() { return input->getValue(); }
