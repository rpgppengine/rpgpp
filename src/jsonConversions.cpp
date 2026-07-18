#include "jsonConversions.hpp"

#include "raylib.h"

void to_json(json &j, const Rectangle &rect) { j = json{rect.x, rect.y, rect.width, rect.height}; }

void from_json(const json &j, Rectangle &rect) {
	j.at(0).get_to(rect.x);
	j.at(1).get_to(rect.y);
	j.at(2).get_to(rect.width);
	j.at(3).get_to(rect.height);
}

void to_json(json &j, const Color &color) {
	std::array<unsigned char, 4> arr = {color.r, color.g, color.b, color.a};
	j = arr;
}

void from_json(const json &j, Color &color) {
	j.at(0).get_to(color.r);
	j.at(1).get_to(color.g);
	j.at(2).get_to(color.b);
	j.at(3).get_to(color.a);
}

void to_json(json &j, const ImageRef &image) {
	j = json::object();
	j["image"] = image.path;
	j["scale"] = image.scale;
}

void from_json(const json &j, ImageRef &image) {
	j.at("image").get_to(image.path);
	j.at("scale").get_to(image.scale);
}

void to_json(json &j, const FontRef &font) {
	j = json::object();
	j["font"] = font.path;
}

void from_json(const json &j, FontRef &font) { j.at("font").get_to(font.path); }

void to_json(json &j, const NPatchInfo &info) {
	j = json{info.source, info.left, info.top, info.right, info.bottom, info.layout};
}

void from_json(const json &j, NPatchInfo &info) {
	j.at(0).get_to(info.source);
	j.at(1).get_to(info.left);
	j.at(2).get_to(info.top);
	j.at(3).get_to(info.right);
	j.at(4).get_to(info.bottom);
	j.at(5).get_to(info.layout);
}

void to_json(json &j, const CallbacksArray &info) { j = info.funcNames; }

void from_json(const json &j, CallbacksArray &info) { info = {j}; }

void to_json(json &j, const UIElementRef &info) { j = json{info.title, info.entityId}; }

void from_json(const json &j, UIElementRef &info) {
	j.at(0).get_to(info.title);
	j.at(1).get_to(info.entityId);
}

void to_json(json &j, const InputC &input) {
	j["up"] = input.upButton;
	j["down"] = input.downButton;
	j["left"] = input.leftButton;
	j["right"] = input.rightButton;
	j["callbacks"] = input.funcNames;
}

void from_json(const json &j, InputC &input) {
	j.at("up").get_to(input.upButton);
	j.at("down").get_to(input.downButton);
	j.at("left").get_to(input.leftButton);
	j.at("right").get_to(input.rightButton);
	j.at("callbacks").get_to(input.funcNames);
}
