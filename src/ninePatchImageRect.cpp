#include "ninePatchImageRect.hpp"

#include "game.hpp"
#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "uiElement.hpp"

NinePatchImageRect::NinePatchImageRect() : NinePatchImageRect({1, 1, 1, 1}) {}

NinePatchImageRect::NinePatchImageRect(Rectangle rect) : UIElement(INTERFACE_NINEPATCHIMAGERECT) {
	this->rect = rect;
	this->scale = 1;
	this->source = "";
	this->npatchInfo = {{0, 0, 20, 20}, 0, 0, 0, 0, 0};
}

void NinePatchImageRect::loadTexture() {
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

void NinePatchImageRect::fromJson(const nlohmann::json &json) {
	this->rect = json.at("rect");
	this->scale = json.at("scale");
	this->source = json.at("source");
	this->npatchInfo = json.at("npatchinfo");

	loadTexture();
}

nlohmann::json NinePatchImageRect::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["scale"] = scale;
	j["source"] = source;
	j["npatchinfo"] = npatchInfo;
	return j;
}

void NinePatchImageRect::fromBin(UIElementBin &bin) {
	rect = std::get<Rectangle>(bin.props["rect"]);
	scale = std::get<int>(bin.props["scale"]);
	source = std::get<std::string>(bin.props["source"]);
	npatchInfo = std::get<NPatchInfo>(bin.props["npatchinfo"]);
}

UIElementBin NinePatchImageRect::dumpBin() {
	UIElementBin bin = UIElement::dumpBin();
	bin.props["rect"] = rect;
	bin.props["scale"] = scale;
	bin.props["source"] = source;
	bin.props["npatchinfo"] = npatchInfo;
	return bin;
}

void NinePatchImageRect::setSource(const std::string &source) {
	this->source = source;
	this->loadTexture();
	this->npatchInfo.source = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
}

void NinePatchImageRect::setTexture(Texture2D texture) { this->texture = texture; }

void NinePatchImageRect::setScale(int scale) { this->scale = scale; }

std::map<std::string, xxx::any_ptr> NinePatchImageRect::getProps() {
	return {{"rect", &rect}, {"scale", &scale}, {"source", &source}, {"npatchinfo", &npatchInfo}};
}

void NinePatchImageRect::update() {}

void NinePatchImageRect::draw() {
	Vector2 origin = {0.0f, 0.0f};

	NPatchInfo info = npatchInfo;
	info.left *= scale;
	info.top *= scale;
	info.right *= scale;
	info.bottom *= scale;

	DrawTextureNPatch(texture, info, rect, origin, 0.0f, WHITE);
}