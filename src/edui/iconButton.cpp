#include "edui/iconButton.hpp"
#include <memory>
#include "edui/helper.hpp"
#include "edui/rlicons.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/widget.hpp"

using namespace edui;

IconButton::IconButton() : Widget() {
	render = std::make_unique<IconButtonRender>();
}

void IconButton::draw() {
	auto &rend = render->as<IconButtonRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	Rectangle iconRect = {rect.x, rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	rectCenter(rect, &iconRect);

	GuiDrawIcon(iconId, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);
}
