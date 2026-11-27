#ifndef _EDITOR_WIDGETS_ANIMATIONFRAMEBUTTON_H
#define _EDITOR_WIDGETS_ANIMATIONFRAMEBUTTON_H

#include "atlasTile.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace editor {
struct AnimationFrameButton : public edui::Widget {
	AtlasTile atlasTile;
	Vector2 atlasTileSize;

	AnimationFrameButton();
	void draw() override;

	void setActiveBorder(bool activeBorder);

private:
	bool activeBorder = false;
};
}  // namespace editor

#endif
