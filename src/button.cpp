#include "button.hpp"

#include <cstdio>

#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "uiElement.hpp"

Button::Button() : Button(Rectangle{0, 0, 20, 20}) {}

Button::Button(Rectangle rect) : UIElement(INTERFACE_BUTTON) {
	focusable = true;
	normalTextColor = label.textColor;
	focusedTextColor = DARKGRAY;
	shownTextColor = label.textColor;
	callbacks[CALLBACK_TRIGGER] = [] { printf("button trigger.. \n"); };
	callbacks[CALLBACK_UNFOCUSED] = [this] { shownTextColor = normalTextColor; };
	callbacks[CALLBACK_FOCUSED] = [this] { shownTextColor = focusedTextColor; };

	this->setRect(rect);
}

void Button::fromJson(const nlohmann::json &json) {
	UIElement::fromJson(json);
	UIElement::fromNavigationJson(json);
	colorRect.fromJson(json.at("colorRect"));
	label.fromJson(json.at("label"));
	normalTextColor = label.textColor;
	shownTextColor = label.textColor;
	focusedTextColor = json["focusedTextColor"];
}

nlohmann::json Button::dumpJson() {
	rect = label.rect;

	auto j = UIElement::dumpNavigationJson();
	j["rect"] = rect;
	auto colorRectDump = colorRect.dumpJson();
	j["colorRect"] = colorRectDump;
	auto labelDump = label.dumpJson();
	j["label"] = labelDump;
	j["focusedTextColor"] = focusedTextColor;
	return j;
}

void Button::fromBin(UIElementBin &bin) {
	UIElement::fromBin(bin);
	UIElement::fromNavigationBin(bin);
	colorRect.fromBin(bin);
	label.fromBin(bin);
	focusedTextColor = std::get<Color>(bin.props["focusedTextColor"]);
}

UIElementBin Button::dumpBin() {
	rect = label.rect;

	auto bin = UIElement::dumpNavigationBin();
	bin.props["rect"] = rect;
	bin.props.merge(colorRect.dumpBin().props);
	bin.props.merge(label.dumpBin().props);
	bin.props["focusedTextColor"] = focusedTextColor;
	return bin;
}

std::map<std::string, xxx::any_ptr> Button::getProps() {
	auto map = UIElement::getProps();
	map.merge(colorRect.getProps());
	map.merge(label.getProps());
	map["focusedTextColor"] = &focusedTextColor;
	map["rect"] = &rect;
	return map;
}

Rectangle Button::getRect() { return label.rect; }

void Button::setRect(const Rectangle &rect) {
	this->label.rect = rect;
	this->colorRect.rect = rect;
}

void Button::setText(const std::string &text) { this->label.setText(text); }

void Button::setTextSize(int size) { label.fontSize = size; }

void Button::setBackgroundColor(Color color) { colorRect.setColor(color); }

void Button::setNormalTextColor(Color color) {
	normalTextColor = color;
	this->label.textColor = normalTextColor;
	shownTextColor = normalTextColor;
}

void Button::setFocusedTextColor(Color color) { focusedTextColor = color; }

void Button::update() {
	label.textColor = shownTextColor;
	colorRect.update();
	label.update();
}

void Button::draw() {
	colorRect.draw();
	label.draw();
}
