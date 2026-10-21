#ifndef _EDUI_COLORRECT_H
#define _EDUI_COLORRECT_H

#include "edui/widget.hpp"

namespace edui {
struct ColorRectRender : public WidgetRender {};

/* A widget which is filled with a single color. */
struct ColorRect : public Widget {
	typedef std::shared_ptr<ColorRect> Ptr;

	Color color = WHITE;

	ColorRect();
	void draw();

	Widget::Ptr clone() { return std::make_shared<ColorRect>(*this); }
};
}  // namespace edui

#endif
