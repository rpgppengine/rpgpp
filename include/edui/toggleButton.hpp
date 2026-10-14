#ifndef _EDUI_TOGGLEBUTTON_H
#define _EDUI_TOGGLEBUTTON_H

#include "edui/iconTextButton.hpp"

namespace edui {
struct ToggleButtonRender : public IconTextButtonRender {
	Color inactiveColor;
	Color inactiveFocusColor;
	Color activeColor;
	Color activeFocusColor;

	void applyTheme(const mINI::INIStructure &struc) {
		IconTextButtonRender::applyTheme(struc);
		INISection section = struc.get("Widget");
		inactiveColor = parseColorString(section.get("BackgroundColor"));
		inactiveFocusColor = parseColorString(section.get("FocusBackgroundColor"));
		activeColor = parseColorString(section.get("SecondaryColor"));
		activeFocusColor = parseColorString(section.get("TertiaryColor"));
	}
};

struct ToggleButton : public IconTextButton {
	bool eventLock = false;

	ToggleButton();
	void applyTheme(const mINI::INIStructure &struc) override;

	void draw() override;

	void leftMouseClicked() override;

	void toggle();
	void setActive();
	void setInactive();

private:
	bool active = false;
};
}  // namespace edui

#endif
