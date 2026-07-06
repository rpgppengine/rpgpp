#include "ui_elements/colorRect.hpp"

ColorRect::ColorRect() : UIElement("ColorRect") { init(); }

void ColorRect::init() {
	UIElement::init();
	props["color"] = RAYWHITE;
}

void ColorRect::update() {}

void ColorRect::draw(Rectangle rect) {
	auto color = std::get<Color>(props["color"]);

	DrawRectangleRec(rect, color);
}
