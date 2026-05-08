#include "label.hpp"

#include <string>

#include "game.hpp"
#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "raymath.h"
#include "uiElement.hpp"

Label::Label() : Label(Rectangle{1, 1, 1, 1}) {}

Label::Label(Rectangle rect) : UIElement(INTERFACE_LABEL) {
	this->rect = rect;
	this->text = "";
	this->textColor = BLACK;
	this->horizontalAlignment = TEXT_ALIGN_CENTRE;
	this->verticalAlignment = TEXT_ALIGN_MIDDLE;
	this->fontName = "LanaPixel";
	this->fontSize = 26;

	if (Game::isUsingBin()) {
		this->font = Game::getResources().getFont(fontName);
	} else {
		this->font = LoadFontEx(fontName.c_str(), fontSize, nullptr, 256);
	}
}

void Label::fromJson(const nlohmann::json &json) {
	rect = json.at("rect");
	text = json.at("text");
	textColor = json.at("textColor");
	horizontalAlignment = json.at("horizontalAlignment");
	verticalAlignment = json.at("verticalAlignment");
	fontName = json.at("fontName");
	fontSize = json.at("fontSize");
}

nlohmann::json Label::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["text"] = text;
	j["textColor"] = textColor;
	j["horizontalAlignment"] = horizontalAlignment;
	j["verticalAlignment"] = verticalAlignment;
	j["fontName"] = fontName;
	j["fontSize"] = fontSize;
	return j;
}

void Label::fromBin(UIElementBin &bin) {
	rect = std::get<Rectangle>(bin.props["rect"]);
	text = std::get<std::string>(bin.props["text"]);
	textColor = std::get<Color>(bin.props["color"]);
	horizontalAlignment = std::get<TextAlignment>(bin.props["horizontalAlignment"]);
	verticalAlignment = std::get<TextAlignment>(bin.props["verticalAlignment"]);
	fontName = std::get<std::string>(bin.props["fontName"]);
	fontSize = std::get<int>(bin.props["fontSize"]);
}

UIElementBin Label::dumpBin() {
	UIElementBin bin = UIElement::dumpBin();
	bin.props["rect"] = rect;
	bin.props["text"] = text;
	bin.props["textColor"] = textColor;
	bin.props["horizontalAlignment"] = horizontalAlignment;
	bin.props["verticalAlignment"] = verticalAlignment;
	bin.props["fontName"] = fontName;
	bin.props["fontSize"] = fontSize;
	return bin;
}

std::map<std::string, xxx::any_ptr> Label::getProps() {
	return {{"rect", &rect},
			{"text", &text},
			{"textColor", &textColor},
			{"horizontalAlignment", &horizontalAlignment},
			{"verticalAlignment", &verticalAlignment},
			{"fontName", &fontName},
			{"fontSize", &fontSize}};
}

void Label::setText(const std::string &text) { this->text = text; }

void Label::update() {}

void Label::draw() {
	printf("%i \n", textColor.r);

	Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(fontSize), 1);

	Vector2 textPos =
		(Vector2){rect.x + Lerp(0.0f, rect.width - textSize.x, (static_cast<float>(horizontalAlignment) * 0.5f)),
				  rect.y + Lerp(0.0f, rect.height - textSize.y, (static_cast<float>(verticalAlignment) * 0.5f))};

	DrawTextEx(font, text.c_str(), textPos, static_cast<float>(fontSize), 1, textColor);
}