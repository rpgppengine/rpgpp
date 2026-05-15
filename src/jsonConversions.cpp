#include "jsonConversions.hpp"

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

void to_json(json &j, const UIElementRef &info) { j = info.title; }

void from_json(const json &j, UIElementRef &info) { info = {j}; }

void to_json(json &j, const ColorRectComponent &color) {
	std::array<unsigned char, 4> arr = {color.color.r, color.color.g, color.color.b, color.color.a};
	j = arr;
}

void from_json(const json &j, ColorRectComponent &color) {
	j.at(0).get_to(color.color.r);
	j.at(1).get_to(color.color.g);
	j.at(2).get_to(color.color.b);
	j.at(3).get_to(color.color.a);
}