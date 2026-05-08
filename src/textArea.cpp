#include "textArea.hpp"

#include <raylib.h>

#include "game.hpp"
#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "uiElement.hpp"

TextArea::TextArea() : TextArea(Rectangle{}) {}

TextArea::TextArea(Rectangle rect) : UIElement(INTERFACE_TEXTAREA) {
	this->rect = rect;
	this->content = "";
}

void TextArea::fromJson(const nlohmann::json &json) {
	rect = json.at("rect");
	content = json.at("content");
}

nlohmann::json TextArea::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["content"] = content;
	return j;
}

void TextArea::fromBin(UIElementBin &bin) {
	rect = std::get<Rectangle>(bin.props["rect"]);
	content = std::get<std::string>(bin.props["content"]);
}

UIElementBin TextArea::dumpBin() {
	UIElementBin bin = UIElement::dumpBin();
	bin.props["rect"] = rect;
	bin.props["content"] = content;
	return bin;
}

std::map<std::string, xxx::any_ptr> TextArea::getProps() { return {{"rect", &rect}, {"content", &content}}; }

void TextArea::setText(const std::string &text) { this->content = text; }

void TextArea::update() {
	// TODO
}

void TextArea::draw() {
	auto textPos = Vector2{0, 0};
	auto charMeasure = Vector2{0, 0};
	for (int i = 0; i < content.size(); i++) {
		putChar(i, &textPos, &charMeasure);
	}
}

void TextArea::putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const {
	charPos->x += charMeasure->x;

	DrawTextPro(Game::getUi().getFont(), TextSubtext(content.c_str(), i, 1), *charPos, Vector2{0, 0}, 0.0f, 13 * 3, 1,
				BLACK);

	*charMeasure = MeasureTextEx(Game::getUi().getFont(), TextSubtext(content.c_str(), i, 1), 13 * 3, 1.0f);
}
