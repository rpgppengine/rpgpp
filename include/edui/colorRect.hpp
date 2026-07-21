#ifndef _EDUI_COLORRECT_H
#define _EDUI_COLORRECT_H

#include "edui/widget.hpp"

namespace edui {
struct ColorRectRender : public WidgetRender {};

struct ColorRect : public Widget {
	typedef std::shared_ptr<ColorRect> Ptr;

	ColorRect();
	void draw();
};
}  // namespace edui

#endif
