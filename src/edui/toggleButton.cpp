#include "edui/toggleButton.hpp"

#include <memory>

#include "edui/gui.hpp"
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

	drawContent();
}

void ToggleButton::leftMouseClicked() {
	IconTextButton::leftMouseClicked();
	if (!eventLock) {
		toggle();
	}
}

void ToggleButton::toggle() {
	active = !active;

	if (active) {
		setActive();
	} else {
		setInactive();
	}

	onToggled.invoke(active);
}

void ToggleButton::setActive() {
	active = true;
	auto &rend = render->as<ToggleButtonRender>();
	rend.bgColor = rend.activeColor;
	rend.focusBgColor = rend.activeFocusColor;
}

void ToggleButton::setInactive() {
	active = false;
	auto &rend = render->as<ToggleButtonRender>();
	rend.bgColor = rend.inactiveColor;
	rend.focusBgColor = rend.inactiveFocusColor;
}
