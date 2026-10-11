#ifndef _EDUI_SYNTAXHIGHLIGHTER_H
#define _EDUI_SYNTAXHIGHLIGHTER_H

#include <string>

#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct SyntaxHighlighterColors : public WidgetRender {
	Color identifier = BLUE;
	Color string = GREEN;
	Color number = ORANGE;
	Color comment = GRAY;
	Color keyword = RED;
	Color operatorC = ORANGE;

	void applyTheme(const mINI::INIStructure &struc) {
		INISection section = struc.get("Code");
		identifier = parseColorString(section.get("Identifier"));
		string = parseColorString(section.get("String"));
		number = parseColorString(section.get("Number"));
		keyword = parseColorString(section.get("Keyword"));
		comment = parseColorString(section.get("Comment"));
		operatorC = parseColorString(section.get("Operator"));
	}
};

struct SyntaxHighlighter {
	SyntaxHighlighterColors colors;

	Color getColor(const std::string &type, Color &defaultColor);
};
}  // namespace edui

#endif
