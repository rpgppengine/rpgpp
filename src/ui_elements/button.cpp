#include "ui_elements/button.hpp"

#include "game.hpp"
#include "raymath.h"

Button::Button() : UIElement("Button") { init(); }

void Button::init() {
	UIElement::init();
	props["text"] = "";
	props["normalTextColor"] = BLACK;
	props["focusedTextColor"] = GRAY;
	props["bgColor"] = RAYWHITE;
	props["horizontalAlignment"] = HorizontalAlignment{};
	props["verticalAlignment"] = VerticalAlignment{};
	props["font"] = FontRef{};
	props["input"] = InputC{};

	callbacks[CALLBACK_FOCUSED] = [this] {
		auto focusedTextColor = std::get<Color>(props["focusedTextColor"]);
		shownTextColor = focusedTextColor;
	};
	callbacks[CALLBACK_UNFOCUSED] = [this] {
		auto normalTextColor = std::get<Color>(props["normalTextColor"]);
		shownTextColor = normalTextColor;
	};
}

void Button::update() {}

void Button::draw(Rectangle rect) {
	auto bgColor = std::get<Color>(props["bgColor"]);

	DrawRectangleRec(rect, bgColor);
	//
	auto text = std::get<std::string>(props["text"]);
	auto textColor = shownTextColor;
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

void Button::config() {
	FontRef &font = std::get<FontRef>(props["font"]);
	loadFont(font.path);
}

void Button::loadFont(const std::string &path) {
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
