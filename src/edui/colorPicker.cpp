#include "edui/colorPicker.hpp"

#include <memory>

#include "edui/colorPanel.hpp"
#include "edui/gui.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

ColorPicker::ColorPicker() : Widget() {
	render = std::make_unique<ColorPickerRender>();
	onClicked.connect([this] {
		if (colorDialog != nullptr) {
			colorDialog->markDelete();
		}
		colorDialog = std::make_shared<edui::ColorPanel>();
		colorDialog->layout.x.offset = layout.x.offset - colorDialog->layout.width.offset + layout.width.offset;
		colorDialog->layout.y.offset = layout.y.offset - colorDialog->layout.height.offset;

		colorDialog->onColorConfirm([this](Color color) {
			onValueChanged.invoke(this->color, color);
			this->color = color;
		});

		colorDialog->setColor(color);

		Gui::instance->add(colorDialog, layerId + 1);
	});
}

void ColorPicker::setValue(const Color &val) { this->color = val; }

Color ColorPicker::getValue() { return color; }

void ColorPicker::draw() {
	auto &rend = render->as<ColorPickerRender>();

	DrawRectangleRec(rect, color);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}
