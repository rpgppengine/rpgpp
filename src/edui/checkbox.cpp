#include "edui/checkbox.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

Checkbox::Checkbox() {
	render = std::make_unique<CheckboxRender>();
	render->padding = 4;
}

void Checkbox::setValue(const bool& val) {
	this->checked = val;
}

bool Checkbox::getValue() {
	return checked;
}

void Checkbox::draw() {
	auto &rend = render->as<ContainerRender>();

	Rectangle effective = rect;
	if (rect.width > rect.height) {
		effective.width = rect.height;
		effective.x += (rect.width - rect.height);
	}

	Rectangle padding = paddingRect(effective, rend.padding);

	DrawRectangleLinesEx(effective, rend.border, rend.currentBorderColor);

	if (checked) {
		DrawRectangleRec(padding, rend.currentBorderColor);
	}
}

void Checkbox::leftMouseClicked() {
	checked = !checked;
	onValueChangedT.invoke(!checked, checked);
	Widget::leftMouseClicked();
}
