#ifndef _EDUI_TOOLTIP
#define _EDUI_TOOLTIP

#include <string>

#include "edui/widget.hpp"

namespace edui {
struct TooltipRender : public WidgetRender {
	float fontSize = -1;
	Color textColor = BLACK;
	float spacing = -1;

	void applyTheme(const mINI::INIStructure &struc) {
		WidgetRender::applyTheme(struc);

		INISection section = struc.get("Label");
		textColor = parseColorString(section.get("TextColor"));
	};
};

struct Tooltip : public Widget {
	Tooltip();

	void draw();
	void setText(const std::string &text);

protected:
	std::string text = "Hello!";
};
}  // namespace edui

#endif
