#include "widgets/values/vector2value.hpp"

#include <memory>

#include "edui/vector2Input.hpp"
#include "raylib.h"

using namespace editor;

Vector2Value::Vector2Value() {
	input = std::make_shared<edui::Vector2Input>();
	input->setSize({1, 0}, {1, 0});
	input->onValueChanged.connect([this](Vector2 a, Vector2 b) { onValueChanged.invoke(a, b); });
	add(input);
}

void Vector2Value::setValue(const Vector2 &value) { input->setValue(value); }

Vector2 Vector2Value::getValue() { return input->getValue(); }
