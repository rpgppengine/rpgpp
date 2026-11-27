#include "widgets/animationFrameButton.hpp"

#include <memory>

#include "edui/helper.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace editor;

AnimationFrameButton::AnimationFrameButton() : edui::Widget() {
	render = std::make_unique<edui::WidgetRender>();
	render->padding = 4;
}

void AnimationFrameButton::setActiveBorder(bool activeBorder) { this->activeBorder = activeBorder; }

void AnimationFrameButton::draw() {
	DrawRectangleRec(rect, render->currentBgColor);

	auto padding = paddingRect(rect, render->padding);

	Rectangle sourceRect = {atlasTile.getAtlasCoords().x * atlasTileSize.x,
							atlasTile.getAtlasCoords().y * atlasTileSize.y, atlasTileSize.x, atlasTileSize.y};
	DrawTexturePro(atlasTile.getTexture(), sourceRect, padding, {0, 0}, 0.0f, WHITE);

	auto finalBorder = render->border;

	if (activeBorder) {
		finalBorder *= 2;
	}

	DrawRectangleLinesEx(rect, finalBorder, render->currentBorderColor);
}
