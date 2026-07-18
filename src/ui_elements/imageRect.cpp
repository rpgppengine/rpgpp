#include "ui_elements/imageRect.hpp"

ImageRect::ImageRect() : UIElement("ImageRect") { init(); }

void ImageRect::init() {
	UIElement::init();
	props["image"] = ImageRef{};
}

void ImageRect::update() {}

void ImageRect::draw(Rectangle rect) {
	ImageRef image = std::get<ImageRef>(props["image"]);

	DrawTexturePro(image.texture,
				   Rectangle{0, 0, static_cast<float>(image.texture.width), static_cast<float>(image.texture.height)},
				   rect, Vector2{0, 0}, 0.0f, WHITE);
}

void ImageRect::config() { scaleImage(std::get<ImageRef>(props["image"]).scale); }

void ImageRect::scaleImage(int scale) {
	ImageRef &imageRef = std::get<ImageRef>(props["image"]);

	imageRef.scale = scale;
	Image image = LoadImage(TextFormat("images/%s", imageRef.path.c_str()));
	imageRef.scale = (imageRef.scale < 1) ? 1 : imageRef.scale;
	ImageResizeNN(&image, image.width * imageRef.scale, image.height * imageRef.scale);
	imageRef.texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void ImageRect::loadImage(const std::string &path) {
	ImageRef &imageRef = std::get<ImageRef>(props["image"]);

	imageRef.path = path;
	scaleImage(imageRef.scale);
}
