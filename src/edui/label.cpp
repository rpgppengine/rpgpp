#include "edui/label.hpp"

#include <cstdio>
#include <memory>

#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/tooltip.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Label::Label() {
	render = std::make_unique<LabelRender>();
	render->padding = 8;
	render->focusBgColor = RAYWHITE;
}

void Label::draw() {
	auto &rend = render->as<LabelRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	Vector2 textSize = MeasureTextEx(*rend.font, shownText.c_str(), totalFontSize, rend.spacing);

	setText(this->text);

	Vector2 textPos;
	textPos.x = paddingRect.x + Lerp(0.0f, paddingRect.width - textSize.x, (static_cast<float>(rend.horiAlign) * 0.5f));
	textPos.y =
		paddingRect.y + Lerp(0.0f, paddingRect.height - textSize.y, (static_cast<float>(rend.vertAlign) * 0.5f));
	textPos.y += rend.fontYOffset;

	DrawTextEx(*rend.font, shownText.c_str(), textPos, totalFontSize, rend.spacing, rend.textColor);

	//DrawRectangleLinesEx({textPos.x, textPos.y, textSize.x, 21}, 2.0f, GREEN);
}

void Label::setText(const std::string &text) {
	this->text = text;

	auto &rend = render->as<LabelRender>();
	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();

	overflown = drawOverflownText(paddingRect, rend.font, rend.fontSize, rend.spacing, text, &shownText);
}

void Label::mouseEntered() {
	if (overflown) {
		this->tooltip = std::make_shared<Tooltip>();
		tooltip->setText(text);
		tooltip->setPosition({0, static_cast<int>(GetMousePosition().x)}, {0, static_cast<int>(GetMousePosition().y)});
		Gui::instance->addTop(tooltip);
	}
	Widget::mouseEntered();
}

void Label::mouseLeft() {
	if (this->tooltip != nullptr) {
		this->tooltip->markDelete();
	}
	Widget::mouseLeft();
}
