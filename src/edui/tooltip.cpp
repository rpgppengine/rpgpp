#include "edui/tooltip.hpp"

#include <memory>
#include <string>

using namespace edui;

Tooltip::Tooltip() {
	render = std::make_unique<TooltipRender>();
	render->padding = 2;
}

void Tooltip::draw() {
	auto &rend = render->as<TooltipRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();

	setText(this->text);

	float totalFontSize = rend.font->baseSize * rend.fontSize;
	Vector2 textPos = {paddingRect.x, paddingRect.y};
	DrawTextEx(*rend.font, text.c_str(), textPos, totalFontSize, rend.spacing, rend.textColor);
}

void Tooltip::setText(const std::string &text) {
	this->text = text;

	auto &rend = render->as<TooltipRender>();

	if (rend.font == nullptr) return;

	float totalFontSize = rend.font->baseSize * rend.fontSize;
	Vector2 textSize = MeasureTextEx(*rend.font, text.c_str(), totalFontSize, rend.spacing);

	layout.width.offset = textSize.x + (rend.padding * 2);
	layout.height.offset = textSize.y + (rend.padding * 2);
}
