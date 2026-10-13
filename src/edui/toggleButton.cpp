#include "edui/toggleButton.hpp"

#include <memory>

#include "edui/iconTextButton.hpp"
#include "edui/rlicons.hpp"

using namespace edui;

ToggleButton::ToggleButton() : IconTextButton() {
	render = std::make_unique<edui::ToggleButtonRender>();
	render->padding = 2;
	auto &rend = render->as<IconTextButtonRender>();
	rend.vertAlign = VerticalAlignment::TEXT_CENTER;
}

void ToggleButton::applyTheme(const mINI::INIStructure &struc) {
	IconTextButton::applyTheme(struc);

	if (active) {
		setActive();
	} else {
		setInactive();
	}
}

void ToggleButton::draw() {
	auto &rend = render->as<IconTextButtonRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	drawText();

	Rectangle iconRect = {rect.x, rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	Rectangle destIconRect = {rect.x, rect.y, static_cast<float>(iconRectSize), static_cast<float>(iconRectSize)};
	rectCenter(destIconRect, &iconRect);

	GuiDrawIcon(iconId, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);

	auto paddingRect = getPaddingRect();
}

void ToggleButton::leftMouseClicked() {
	IconTextButton::leftMouseClicked();
	toggle();
}

void ToggleButton::toggle() {
	active = !active;

	if (active) {
		setActive();
	} else {
		setInactive();
	}
}

void ToggleButton::setActive() {
	auto &rend = render->as<ToggleButtonRender>();
	rend.bgColor = rend.activeColor;
	rend.focusBgColor = rend.activeFocusColor;
}

void ToggleButton::setInactive() {
	auto &rend = render->as<ToggleButtonRender>();
	rend.bgColor = rend.inactiveColor;
	rend.focusBgColor = rend.inactiveFocusColor;
}
