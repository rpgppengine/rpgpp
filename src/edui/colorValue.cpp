#include "edui/colorValue.hpp"

#include <memory>

#include "edui/colorPanel.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

ColorValue::ColorValue() : Widget() {
	render = std::make_unique<ColorValueRender>();
	onClicked.connect([this] {
		if (colorDialog != nullptr) {
			colorDialog->markDelete();
		}
		colorDialog = std::make_shared<edui::ColorPanel>();
		colorDialog->layout.x.offset = layout.x.offset - colorDialog->layout.width.offset + layout.width.offset;
		colorDialog->layout.y.offset = layout.y.offset - colorDialog->layout.height.offset;

		colorDialog->onColorConfirm([this](Color color) {
			onValueChangedT.invoke(this->color, color);
			this->color = color;
		});

		colorDialog->setColor(color);

		Gui::instance->addTop(colorDialog);
	});
}

void ColorValue::setValue(const Color &val) { this->color = val; }

Color ColorValue::getValue() { return color; }

void ColorValue::draw() {
	auto &rend = render->as<ColorValueRender>();

	DrawRectangleRec(rect, color);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}
