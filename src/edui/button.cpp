#include "edui/button.hpp"

#include <memory>

#include "edui/label.hpp"
#include "raylib.h"

using namespace edui;

Button::Button() : Label() {
	render = std::make_unique<ButtonRender>();
	render->padding = 2;
	render->focusBgColor = LIGHTGRAY;
}

void Button::draw() {
	auto &rend = render->as<LabelRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	drawText();
}
