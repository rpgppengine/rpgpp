#ifndef _EDUI_ICONTEXTBUTTON_H
#define _EDUI_ICONTEXTBUTTON_H

#include "edui/label.hpp"
#include "edui/widget.hpp"
namespace edui {
struct IconTextButtonRender : public LabelRender {};

struct IconTextButton : public Label {
	IconTextButton();

	int iconId = 0;

	void draw() override;
	Rectangle getContentRect() override;
};
}  // namespace edui

#endif
