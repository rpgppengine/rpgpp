#include "edui/label.hpp"

#include <memory>

#include "edui/gui.hpp"
#include "edui/tooltip.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Label::Label() {
	render = std::make_unique<LabelRender>();
	render->padding = 8;
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

	DrawTextEx(*rend.font, shownText.c_str(), textPos, totalFontSize, rend.spacing, rend.textColor);
}

void Label::setText(const std::string &text) {
	this->text = text;

	auto &rend = render->as<LabelRender>();
	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();

	float totalFontSize = rend.font->baseSize * rend.fontSize;
	Vector2 textSize = MeasureTextEx(*rend.font, text.c_str(), totalFontSize, rend.spacing);

	if (textSize.x > paddingRect.width) {
		int result = text.size() - 1;
		for (int i = 1; i < text.size(); i++) {
			std::string subStr = TextSubtext(text.c_str(), 0, i);
			Vector2 testTextSize = MeasureTextEx(*rend.font, subStr.c_str(), totalFontSize, rend.spacing);
			if (testTextSize.x > paddingRect.width) {
				result = i - 1;
				break;
			}
		}

		this->shownText = TextSubtext(text.c_str(), 0, result - 2);
		this->shownText = this->shownText.append("...");

		overflown = true;
	} else {
		this->shownText = text;

		overflown = false;
	}
}

void Label::mouseEntered() {
	if (overflown) {
		this->tooltip = std::make_shared<Tooltip>();
		tooltip->setText(text);
		tooltip->setPosition({0, static_cast<int>(GetMousePosition().x)}, {0, static_cast<int>(GetMousePosition().y)});
		Gui::instance->addTop(tooltip);
	}
}

void Label::mouseLeft() {
	if (this->tooltip != nullptr) {
		this->tooltip->markDelete();
	}
}
