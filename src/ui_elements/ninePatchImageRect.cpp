#include "ui_elements/ninePatchImageRect.hpp"

NinePatchImageRect::NinePatchImageRect() : UIElement("NinePatchImageRect") { init(); }

void NinePatchImageRect::init() {
	UIElement::init();
	props["image"] = ImageRef{};
	props["npatchInfo"] = NPatchInfo{{0, 0, 0, 0}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
}

void NinePatchImageRect::update() {}

void NinePatchImageRect::draw(Rectangle rect) {
	ImageRef image = std::get<ImageRef>(props["image"]);
	NPatchInfo npatchInfo = std::get<NPatchInfo>(props["npatchInfo"]);

	Vector2 origin = {0.0f, 0.0f};

	NPatchInfo info = npatchInfo;
	info.source = {0, 0, static_cast<float>(image.texture.width), static_cast<float>(image.texture.height)};
	info.left *= image.scale;
	info.top *= image.scale;
	info.right *= image.scale;
	info.bottom *= image.scale;

	DrawTextureNPatch(image.texture, info, rect, origin, 0.0f, WHITE);
}

void NinePatchImageRect::config() { scaleImage(std::get<ImageRef>(props["image"]).scale); }

void NinePatchImageRect::scaleImage(int scale) {
	ImageRef &imageRef = std::get<ImageRef>(props["image"]);

	imageRef.scale = scale;
	Image image = LoadImage(TextFormat("images/%s", imageRef.path.c_str()));
	imageRef.scale = (imageRef.scale < 1) ? 1 : imageRef.scale;
	ImageResizeNN(&image, image.width * imageRef.scale, image.height * imageRef.scale);
	imageRef.texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void NinePatchImageRect::loadImage(const std::string &path) {
	ImageRef &imageRef = std::get<ImageRef>(props["image"]);

	imageRef.path = path;
	scaleImage(imageRef.scale);
}
