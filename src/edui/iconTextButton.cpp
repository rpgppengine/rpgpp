#include "edui/iconTextButton.hpp"

#include <memory>

#include "edui/gui.hpp"
#include "edui/label.hpp"
#include "edui/rlicons.hpp"
#include "raylib.h"

using namespace edui;

IconTextButton::IconTextButton() {
	render = std::make_unique<IconTextButtonRender>();
	render->padding = 2;
	auto &rend = render->as<IconTextButtonRender>();
	rend.vertAlign = VerticalAlignment::TEXT_CENTER;
}

void IconTextButton::calcRect(Rectangle &base) { Label::calcRect(base); }

Rectangle IconTextButton::getContentRect() {
	if (iconId == 0) {
		return Label::getContentRect();
	} else {
		return {rect.x + rect.height, rect.y, rect.width - iconRectSize, static_cast<float>(iconRectSize)};
	}
}

void IconTextButton::draw() {
	auto &rend = render->as<IconTextButtonRender>();

	Label::draw();

	Rectangle iconRect = {rect.x, rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	Rectangle destIconRect = {rect.x, rect.y, static_cast<float>(iconRectSize), static_cast<float>(iconRectSize)};
	rectCenter(destIconRect, &iconRect);

	GuiDrawIcon(iconId, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);

	auto paddingRect = getPaddingRect();
}

void IconTextButton::resizeToFit() {
	auto &rend = render->as<LabelRender>();

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 textSize = MeasureTextEx(Gui::instance->font, text.c_str(), totalFontSize, spacing);

	float totalWidth = textSize.x + (rend.padding * 2);

	if (iconId > 0) {
		totalWidth += iconRectSize + (rend.padding * 2);
	}

	layout.width.offset = totalWidth;
	layout.width.scale = 0;
}
