#ifndef _EDUI_ICONBUTTON_H
#define _EDUI_ICONBUTTON_H

#include "edui/widget.hpp"
namespace edui {
struct IconButtonRender : public WidgetRender {};

struct IconButton : public Widget {
	IconButton();

	int iconId = 0;

	void draw() override;
};
}  // namespace edui

#endif
