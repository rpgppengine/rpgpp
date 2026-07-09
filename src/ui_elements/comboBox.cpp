#include "ui_elements/comboBox.hpp"
#include "game.hpp"
#include "raymath.h"

ComboBox::ComboBox() : UIElement("ComboBox") {
	init();
}

void ComboBox::init() {
	props["value"] = 0;
	props["stringValue"] = std::string{};
	props["values"] = StringVector{"one", "two", "three"};
	props["font"] = FontRef{};
	props["normalTextColor"] = RAYWHITE;
	props["focusedTextColor"] = GRAY;
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

void ComboBox::config() {
	FontRef &font = std::get<FontRef>(props["font"]);
	loadFont(font.path);

	Color normalTextColor = std::get<Color>(props["normalTextColor"]);
	this->shownTextColor = normalTextColor;

	StringVector* vec = std::get_if<StringVector>(&props["values"]);
	int index = std::get<int>(props["value"]);

	std::string newStringValue = (*vec)[index];
	props["stringValue"] = newStringValue;
}

void ComboBox::update() {}

void ComboBox::draw(Rectangle rect) {
	auto text = std::get<std::string>(props["stringValue"]);
	auto textColor = shownTextColor;
	TextAlignment horizontalAlignment = TEXT_ALIGN_MIDDLE;
	TextAlignment verticalAlignment = TEXT_ALIGN_CENTRE;
	auto font = std::get<FontRef>(props["font"]);

	Vector2 textSize = MeasureTextEx(font.font, text.c_str(), static_cast<float>(font.fontSize), 1);

	Vector2 textPos;
	textPos.x = rect.x + Lerp(0.0f, rect.width - textSize.x, (static_cast<float>(horizontalAlignment) * 0.5f));
	textPos.y = rect.y + Lerp(0.0f, rect.height - textSize.y, (static_cast<float>(verticalAlignment) * 0.5f));

	DrawTextEx(font.font, text.c_str(), textPos, static_cast<float>(font.fontSize), 1, textColor);
}

void ComboBox::onNotify(Event event) {
	if (event.hold) return;

	StringVector* vec = std::get_if<StringVector>(&props["values"]);
	int* index = std::get_if<int>(&props["value"]);

	if (event.key == KEY_LEFT) {
		if (*index > 0) {
			(*index)--;
		} else {
			(*index) = (vec->size() - 1);
		}
	}
	if (event.key == KEY_RIGHT) {
		if (*index < (vec->size() - 1)) {
			(*index)++;
		} else {
			(*index) = 0;
		}
	}

	std::string newStringValue = (*vec)[*index];
	props["stringValue"] = newStringValue;

	UIElement::onNotify(event);
}

void ComboBox::loadFont(const std::string &path) {
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
