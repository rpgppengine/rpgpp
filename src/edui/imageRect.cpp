#include "edui/imageRect.hpp"

#include <memory>

#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

ImageRect::ImageRect() : Widget() { render = std::make_unique<ImageRectRender>(); }

void ImageRect::draw() {
	auto &rend = render->as<ImageRectRender>();

	if (IsTextureValid(texture)) {
		DrawTexturePro(texture, sourceRect, rect, {0, 0}, 0.0f, WHITE);
	}
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void ImageRect::setTexture(Texture texture) {
	this->texture = texture;
	this->sourceRect = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
}

void ImageRect::resizeToTexture() {
	this->layout.width.offset = texture.width;
	this->layout.height.offset = texture.height;
}
