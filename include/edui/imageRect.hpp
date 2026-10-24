#ifndef _EDUI_IMAGERECT_H
#define _EDUI_IMAGERECT_H

#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
struct ImageRectRender : public WidgetRender {};

/* A widget which displays a texture. */
struct ImageRect : public Widget {
	Rectangle sourceRect;

	ImageRect();

	void draw() override;

	void setTexture(Texture texture);
	void resizeToTexture();

private:
	Texture texture;
};
}  // namespace edui

#endif
