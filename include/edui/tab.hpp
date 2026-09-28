#ifndef _EDUI_TAB_H
#define _EDUI_TAB_H

#include "edui/iconTextButton.hpp"
namespace edui {
struct TabRender : public IconTextButtonRender {
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

struct Tab : public IconTextButton {
	Tab();

	virtual void setActive();
	virtual void setInactive();
};
}  // namespace edui

#endif
