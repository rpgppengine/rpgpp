#include "widgets/values/stringValue.hpp"

#include <memory>

#include "edui/textBox.hpp"

using namespace editor;

StringValue::StringValue() {
	textBox = std::make_shared<edui::TextBox>();
	textBox->setSize({1, 0}, {1, 0});
	textBox->onValueChanged.connect([this](std::string a, std::string b) { onValueChanged.invoke(a, b); });
	add(textBox);
}

void StringValue::setValue(const std::string &value) { textBox->setValue(value); }

std::string StringValue::getValue() { return textBox->getValue(); }
