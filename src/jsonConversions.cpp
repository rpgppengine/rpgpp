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

// components

void to_json(json &j, const InputComponent &input) {
	j["up"] = input.upButton;
	j["down"] = input.downButton;
	j["left"] = input.leftButton;
	j["right"] = input.rightButton;
}

void from_json(const json &j, InputComponent &input) {
	j.at("up").get_to(input.upButton);
	j.at("down").get_to(input.downButton);
	j.at("left").get_to(input.leftButton);
	j.at("right").get_to(input.rightButton);
}

void to_json(json &j, const LabelComponent &label) {
	j["text"] = label.text;
	j["textColor"] = label.textColor;
	j["horizontalAlignment"] = label.horizontalAlignment;
	j["verticalAlignment"] = label.verticalAlignment;
	j["fontName"] = label.fontName;
	j["fontSize"] = label.fontSize;
}

void from_json(const json &j, LabelComponent &label) {
	j.at("text").get_to(label.text);
	j.at("textColor").get_to(label.textColor);
	j.at("horizontalAlignment").get_to(label.horizontalAlignment);
	j.at("verticalAlignment").get_to(label.verticalAlignment);
	j.at("fontName").get_to(label.fontName);
	j.at("fontSize").get_to(label.fontSize);
}

void to_json(json &j, const TextAreaComponent &textArea) {
	j["text"] = textArea.text;
	j["textColor"] = textArea.textColor;
	j["fontName"] = textArea.fontName;
	j["fontSize"] = textArea.fontSize;
}

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

void from_json(const json &j, TextAreaComponent &textArea) {
	j.at("text").get_to(textArea.text);
	j.at("textColor").get_to(textArea.textColor);
	j.at("fontName").get_to(textArea.fontName);
	j.at("fontSize").get_to(textArea.fontSize);
}

void to_json(json &j, const ImageRectComponent &image) {
	j["source"] = image.source;
	j["scale"] = image.scale;
}

void from_json(const json &j, ImageRectComponent &image) {
	j.at("source").get_to(image.source);
	j.at("scale").get_to(image.scale);
}

void to_json(json &j, const NinePatchImageRectComponent &ninePatch) {
	j["source"] = ninePatch.source;
	j["scale"] = ninePatch.scale;
	j["npatchinfo"] = ninePatch.npatchInfo;
}

void from_json(const json &j, NinePatchImageRectComponent &ninePatch) {
	j.at("source").get_to(ninePatch.source);
	j.at("scale").get_to(ninePatch.scale);
	j.at("npatchinfo").get_to(ninePatch.npatchInfo);
}

void to_json(json &j, const DialogueComponent &dialogue) {
	j["text"] = dialogue.text;
	j["textColor"] = dialogue.textColor;
}

void from_json(const json &j, DialogueComponent &dialogue) {
	j.at("text").get_to(dialogue.text);
	j.at("textColor").get_to(dialogue.textColor);
}
