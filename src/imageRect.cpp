#include "imageRect.hpp"

#include <raylib.h>

#include <game.hpp>

#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "uiElement.hpp"

ImageRect::ImageRect() : ImageRect(Rectangle{}) {}

ImageRect::ImageRect(Rectangle rect) : UIElement(INTERFACE_IMAGERECT) {
	this->rect = rect;
	this->source = "";
	this->scale = 1;
}

void ImageRect::loadTexture() {
	if (Game::instancePtr() != nullptr) {
		std::string query = TextFormat("%s-%i", source.c_str(), scale);
		if (Game::getResources().textureExists(query) || scale > 1) {
			texture = Game::getResources().getTexture(query);
		} else {
			texture = Game::getResources().getTexture(source);
		}
	} else {
		if (scale > 1) {
			Image image = LoadImage(TextFormat("images/%s", source.c_str()));
			ImageResizeNN(&image, image.width * scale, image.height * scale);
			texture = LoadTextureFromImage(image);
			UnloadImage(image);
		} else {
			texture = LoadTexture(TextFormat("images/%s", source.c_str()));
		}
	}
}

void ImageRect::fromJson(const nlohmann::json &json) {
	rect = json.at("rect");
	source = json.at("source");
	scale = json.at("scale");
	loadTexture();
}

nlohmann::json ImageRect::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["source"] = source;
	j["scale"] = scale;
	return j;
}

void ImageRect::fromBin(UIElementBin &bin) {
	rect = std::get<Rectangle>(bin.props["rect"]);
	source = std::get<std::string>(bin.props["source"]);
	scale = std::get<int>(bin.props["scale"]);
	loadTexture();
}

UIElementBin ImageRect::dumpBin() {
	UIElementBin bin = UIElement::dumpBin();
	bin.props["rect"] = rect;
	bin.props["source"] = source;
	bin.props["scale"] = scale;
	return bin;
}

std::map<std::string, xxx::any_ptr> ImageRect::getProps() {
	return {{"rect", &rect}, {"source", &source}, {"scale", &scale}};
}

void ImageRect::setSource(const std::string &source) {
	this->source = source;
	this->loadTexture();
}

void ImageRect::setTexture(Texture2D texture) { this->texture = texture; }

void ImageRect::update() {}

void ImageRect::draw() {
	DrawTexturePro(texture, Rectangle{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
				   rect, Vector2{0, 0}, 0.0f, WHITE);
}
