#include "widgets/values/rectangleValue.hpp"

#include <memory>

#include "edui/rectangleInput.hpp"
#include "raylib.h"

using namespace editor;

RectangleValue::RectangleValue() {
	preferredSize = 2;

	input = std::make_shared<edui::RectangleInput>();
	input->setSize({1, 0}, {1, 0});
	input->onValueChanged.connect([this](Rectangle a, Rectangle b) { onValueChanged.invoke(a, b); });
	add(input);
}

Rectangle RectangleValue::getValue() { return input->getValue(); }

void RectangleValue::setValue(const Rectangle &value) { input->setValue(value); }
