#include "edui/colorRect.hpp"

using namespace edui;

ColorRect::ColorRect() { render = std::make_unique<ColorRectRender>(); }

void ColorRect::draw() {
	auto &rend = render->as<ColorRectRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}
