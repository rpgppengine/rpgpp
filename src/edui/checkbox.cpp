#include "edui/checkbox.hpp"
#include <memory>
#include "edui/container.hpp"
#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

Checkbox::Checkbox() {
	render = std::make_unique<ContainerRender>();
	render->padding = 4;
}

void Checkbox::draw() {
	auto& rend = render->as<ContainerRender>();

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
	valueChanged.invoke(!checked, checked);
	Widget::leftMouseClicked();
}
