#include "edui/label.hpp"
#include "raymath.h"

using namespace edui;

Label::Label() {
	render = std::make_unique<LabelRender>();
	render->padding = 8;
}

void Label::draw() {
	auto& rend = render->as<LabelRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();

	float totalFontSize = static_cast<float>(rend.font->baseSize * rend.fontSize);

	Vector2 textSize = MeasureTextEx(*rend.font, text.c_str(), totalFontSize, 1);

	Vector2 textPos;
	textPos.x = paddingRect.x + Lerp(0.0f, paddingRect.width - textSize.x, (static_cast<float>(rend.horiAlign) * 0.5f));
	textPos.y = paddingRect.y + Lerp(0.0f, paddingRect.height - textSize.y, (static_cast<float>(rend.vertAlign) * 0.5f));

	DrawTextEx(*rend.font, text.c_str(), textPos, totalFontSize, 1, rend.textColor);
}
