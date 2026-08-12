#include "edui/iconTextButton.hpp"

#include <memory>

#include "edui/label.hpp"
#include "edui/rlicons.hpp"
#include "raylib.h"

using namespace edui;

IconTextButton::IconTextButton() { render = std::make_unique<IconTextButtonRender>();
	auto &rend = render->as<IconTextButtonRender>();
	rend.vertAlign = VerticalAlignment::TEXT_CENTER;
}

Rectangle IconTextButton::getContentRect() {
	return {rect.x + rect.height, rect.y, rect.width - rect.height, rect.height};
}

void IconTextButton::draw() {
	auto &rend = render->as<IconTextButtonRender>();

	Label::draw();

	Rectangle iconRect = {rect.x, rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	Rectangle destIconRect = {rect.x, rect.y, rect.height, rect.height};
	rectCenter(destIconRect, &iconRect);

	GuiDrawIcon(iconId, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);

	auto paddingRect = getPaddingRect();
	//DrawRectangleLinesEx(paddingRect, 2.0f, MAROON);
}
