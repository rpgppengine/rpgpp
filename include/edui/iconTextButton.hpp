#ifndef _EDUI_ICONTEXTBUTTON_H
#define _EDUI_ICONTEXTBUTTON_H

#include "edui/label.hpp"
#include "edui/widget.hpp"

namespace edui {
struct IconTextButtonRender : public LabelRender {};

struct IconTextButton : public Label {
	IconTextButton();

	int iconId = 0;
	int iconRectSize = 22;

	void draw() override;
	void calcRect(Rectangle &base) override;
	Rectangle getContentRect() override;

	void resizeToFit();
};
}  // namespace edui

#endif
