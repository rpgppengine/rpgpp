#include "widgets/values/colorValue.hpp"

#include <memory>

#include "edui/colorPicker.hpp"
#include "raylib.h"

using namespace editor;

ColorValue::ColorValue() {
	picker = std::make_shared<edui::ColorPicker>();
	picker->setSize({1, 0}, {1, 0});
	picker->onValueChanged.connect([this](Color a, Color b) { onValueChanged.invoke(a, b); });
	add(picker);
}

void ColorValue::setValue(const Color &value) { picker->setValue(value); }

Color ColorValue::getValue() { return picker->getValue(); }
