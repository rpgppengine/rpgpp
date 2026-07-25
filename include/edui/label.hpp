#ifndef _EDUI_LABEL_H
#define _EDUI_LABEL_H

#include <string>

#include "edui/widget.hpp"

namespace edui {
enum class HorizontalAlignment { TEXT_LEFT, TEXT_MIDDLE, TEXT_RIGHT };

enum class VerticalAlignment { TEXT_TOP, TEXT_CENTER, TEXT_BOTTOM };

struct LabelRender : public WidgetRender {
	HorizontalAlignment horiAlign = HorizontalAlignment::TEXT_MIDDLE;
	VerticalAlignment vertAlign = VerticalAlignment::TEXT_CENTER;
	float fontSize = 2;
	Color textColor = BLACK;
};

struct Label : public Widget {
	std::string text = "Hello!";

	Label();
	void draw();
};
}  // namespace edui

#endif
