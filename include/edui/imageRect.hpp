#ifndef _EDUI_IMAGERECT_H
#define _EDUI_IMAGERECT_H

#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
struct ImageRectRender : public WidgetRender {};

struct ImageRect : public Widget {
	Texture texture;
	Rectangle sourceRect;

	ImageRect();

	void draw() override;

	void setTexture(Texture texture);
	void resizeToTexture();
};
}  // namespace edui

#endif
