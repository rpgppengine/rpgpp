#include "ui_elements/textBox.hpp"

#include "game.hpp"
#include "raymath.h"

TextBox::TextBox() : UIElement("TextBox") { init(); }

void TextBox::init() {
	props["text"] = std::string{};
	props["normalTextColor"] = RAYWHITE;
	props["focusedTextColor"] = GRAY;
	props["horizontalAlignment"] = HorizontalAlignment{TEXT_ALIGN_MIDDLE};
	props["verticalAlignment"] = VerticalAlignment{TEXT_ALIGN_CENTRE};
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

void TextBox::config() {
	FontRef &font = std::get<FontRef>(props["font"]);
	loadFont(font.path);

	Color normalTextColor = std::get<Color>(props["normalTextColor"]);
	this->shownTextColor = normalTextColor;
}

void TextBox::update() {
	if (debounce > 0) {
		debounce--;
	}
}

void TextBox::onNotify(Event event) {
	std::string *text = std::get_if<std::string>(&props["text"]);

	if (debounce == 0) {
		if (event.key == KEY_LEFT) {
			if (cursorPosition > 0) {
				cursorPosition--;
			}
		} else if (event.key == KEY_RIGHT) {
			if (cursorPosition < text->length()) {
				cursorPosition++;
			}
		} else if (event.key == KEY_BACKSPACE) {
			if (!text->empty()) {
				text->erase(cursorPosition - 1, 1);
				cursorPosition--;
			}
		}

		debounce = 10;
	}

	int key = GetCharPressed();
	while (key > 0) {
		if (key >= 32 && key <= 125) {
			text->insert(text->begin() + cursorPosition, 1, static_cast<char>(key));
			cursorPosition++;
		}

		key = GetCharPressed();
	}

	UIElement::onNotify(event);
}

void TextBox::draw(Rectangle rect) {
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

	std::string subText = TextSubtext(text.c_str(), 0, cursorPosition);
	Vector2 cursorMeasure;
	if (cursorPosition == 0) {
		cursorMeasure = MeasureTextEx(font.font, text.c_str(), static_cast<float>(font.fontSize), 1);
		cursorMeasure.x = 0;
	} else {
		cursorMeasure = MeasureTextEx(font.font, subText.c_str(), static_cast<float>(font.fontSize), 1);
	}
	if (text.empty()) {
		cursorMeasure = MeasureTextEx(font.font, "1", static_cast<float>(font.fontSize), 1);
		cursorMeasure.x = 0;
		textSize.y = cursorMeasure.y;
	}

	Vector2 cursorPos;
	cursorPos.x = rect.x + cursorMeasure.x +
				  Lerp(0.0f, rect.width - textSize.x, (static_cast<float>(horizontalAlignment) * 0.5f));
	cursorPos.y = rect.y + Lerp(0.0f, rect.height - textSize.y, (static_cast<float>(verticalAlignment) * 0.5f));

	Rectangle cursorRect = {cursorPos.x, cursorPos.y, 8, cursorMeasure.y};
	DrawRectangleRec(cursorRect, textColor);
}

void TextBox::loadFont(const std::string &path) {
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
