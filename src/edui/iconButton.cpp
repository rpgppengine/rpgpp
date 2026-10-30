#include "edui/iconButton.hpp"

#include <memory>

#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/rlicons.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/tooltip.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

IconButton::IconButton() : Widget() { render = std::make_unique<IconButtonRender>(); }

void IconButton::translate() {
	Widget::translate();
	if (hasTooltip) {
		setTooltip();
	}
}

void IconButton::draw() {
	auto &rend = render->as<IconButtonRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	Rectangle iconRect = {rect.x, rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	rectCenter(rect, &iconRect);

	GuiDrawIcon(iconId, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);
}

void IconButton::setTooltip() {
	hasTooltip = true;
	tooltipText = getTranslation(translationId.c_str(), tooltipText);
}

void IconButton::setTooltip(const std::string &text) {
	tooltipText = text;
	hasTooltip = true;
}

void IconButton::mouseEntered() {
	if (hasTooltip) {
		tooltip = std::make_shared<Tooltip>();
		tooltip->setText(tooltipText);
		tooltip->setPosition({0, static_cast<int>(GetMousePosition().x)}, {0, static_cast<int>(GetMousePosition().y)});
		Gui::instance->add(tooltip, layerId + 1);
	}
	Widget::mouseEntered();
}

void IconButton::mouseLeft() {
	if (tooltip != nullptr) {
		tooltip->markDelete();
	}
	Widget::mouseLeft();
}
