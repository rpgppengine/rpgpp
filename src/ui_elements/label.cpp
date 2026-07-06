#include "ui_elements/label.hpp"

#include "game.hpp"
#include "raymath.h"

Label::Label() : UIElement("Label") { init(); }

void Label::init() {
	UIElement::init();
	props["text"] = "";
	props["textColor"] = BLACK;
	props["horizontalAlignment"] = HorizontalAlignment{};
	props["verticalAlignment"] = VerticalAlignment{};
	props["font"] = FontRef{};
}

void Label::update() {}

void Label::draw(Rectangle rect) {
	auto text = std::get<std::string>(props["text"]);
	auto textColor = std::get<Color>(props["textColor"]);
	TextAlignment horizontalAlignment =
		static_cast<TextAlignment>(std::get<HorizontalAlignment>(props["horizontalAlignment"]).val);
	TextAlignment verticalAlignment =
		static_cast<TextAlignment>(std::get<VerticalAlignment>(props["verticalAlignment"]).val);
	auto font = std::get<FontRef>(props["font"]);

	Vector2 textSize = MeasureTextEx(font.font, text.c_str(), static_cast<float>(font.fontSize), 1);

	Vector2 textPos;
	textPos.x = rect.x + Lerp(0.0f, rect.width - textSize.x, (static_cast<float>(horizontalAlignment) * 0.5f));
	textPos.y = rect.y + Lerp(0.0f, rect.height - textSize.y, (static_cast<float>(verticalAlignment) * 0.5f));

	DrawTextEx(font.font, text.c_str(), textPos, static_cast<float>(font.fontSize), 1, textColor);
}

void Label::config() {
	FontRef &font = std::get<FontRef>(props["font"]);
	loadFont(font.path);
}

void Label::loadFont(const std::string &path) {
	FontRef &font = std::get<FontRef>(props["font"]);

	std::string fullPath = TextFormat("fonts/%s", font.path.c_str());
	font.path = path;

	if (Game::isUsingBin()) {
		if (font.path.empty()) {
			font.font = Game::getResources().getFont("LanaPixel");
		} else {
			font.font = Game::getResources().getFont(GetFileNameWithoutExt(path.c_str()));
		}
	} else {
		if (font.path.empty()) {
			auto fontPaths = LoadDirectoryFiles("fonts/");
			if (fontPaths.count > 0) {
				auto fontPath = fontPaths.paths[0];
				fullPath = fontPath;
			}
		}
		font.font = LoadFontEx(fullPath.c_str(), font.fontSize, nullptr, 256);
	}
}
