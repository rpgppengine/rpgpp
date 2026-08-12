#ifndef _EDUI_LABEL_H
#define _EDUI_LABEL_H

#include <memory>
#include <string>

#include "edui/tooltip.hpp"
#include "edui/widget.hpp"

namespace edui {
enum class HorizontalAlignment { TEXT_LEFT, TEXT_MIDDLE, TEXT_RIGHT };

enum class VerticalAlignment { TEXT_TOP, TEXT_CENTER, TEXT_BOTTOM };

struct LabelRender : public WidgetRender {
	HorizontalAlignment horiAlign = HorizontalAlignment::TEXT_LEFT;
	VerticalAlignment vertAlign = VerticalAlignment::TEXT_TOP;
	float fontSize = 1;
	Color textColor = BLACK;
	float spacing = 0.05f;
	float fontYOffset = 0.0f;
};

struct Label : public Widget {
	std::shared_ptr<Tooltip> tooltip = nullptr;

	bool overflown = false;

	Label();
	void draw();

	void setText(const std::string &text);

	void mouseEntered();
	void mouseLeft();

protected:
	std::string shownText = "Hello!";
	std::string text = "Hello!";
};
}  // namespace edui

#endif
