#include "edui/textEdit.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string_view>

#include "edui/gui.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

TextEdit::TextEdit() {
	focusable = true;
	render = std::make_unique<TextEditRender>();
	render->padding = 2;
}

TextEdit::~TextEdit() { unload(); }

void TextEdit::unload() {
	if (lines != nullptr) {
		MemFree(lines);
	}
}

void TextEdit::setText(const std::string &newText) {
	this->text = newText;
	lines = LoadTextLines(this->text.c_str(), &rowCount);
}

void TextEdit::update() {
	if (debounce > 0) {
		debounce--;
	}
}

void TextEdit::draw() {
	auto &rend = render->as<TextEditRender>();

	DrawRectangleRec(rect, rend.bgColor);

	Vector2 textBegin = {rect.x + rend.padding, rect.y + rend.padding};

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;
	DrawTextEx(*rend.font, text.c_str(), textBegin, totalFontSize, spacing, rend.borderColor);

	if (isFocused) {
		drawCursor();
	}

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void TextEdit::drawCursor() {
	auto &rend = render->as<TextEditRender>();
	DrawRectangleRec(cursorRect, rend.borderColor);
}

void TextEdit::setCursorFromMouse() {
	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 mousePos = GetMousePosition();
	mousePos = Vector2Subtract(mousePos, {rect.x + rend.padding, rect.y + rend.padding});

	if (mousePos.x < 0 || mousePos.y < 0) return;

	int row = static_cast<int>(floor(mousePos.y / totalFontSize));
	std::string_view line = lines[row];

	int col = 0;
	for (auto i = 0; i <= line.size(); i++) {
		std::string sub = TextSubtext(line.data(), 0, i);
		Vector2 subMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);
		if (subMeasure.x > mousePos.x) {
			col = i - 1;
			break;
		}
	}

	Vector2 measure = MeasureTextEx(*rend.font, line.data(), totalFontSize, spacing);
	if (mousePos.x > measure.x) {
		col = line.size();
	}

	cursorPos.row = row;
	cursorPos.column = col;

	std::string sub = TextSubtext(line.data(), 0, col);
	Vector2 subMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);
	cursorRect.x = rect.x + rend.padding + subMeasure.x;
	cursorRect.y = rect.y + rend.padding + (totalFontSize * row);
	cursorRect.height = totalFontSize;

	this->charPos = calcCharPos(row, col);

	if (charPos != text.size()) {
		printf("%zu %c \n", charPos, text.at(charPos));
	} else {
		printf("end of text \n");
	}
}

void TextEdit::setCursorRect() {
	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	std::string_view line = lines[cursorPos.row];

	std::string sub = TextSubtext(line.data(), 0, cursorPos.column);
	Vector2 subMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);
	cursorRect.x = rect.x + rend.padding + subMeasure.x;
	cursorRect.y = rect.y + rend.padding + (totalFontSize * cursorPos.row);
	cursorRect.height = totalFontSize;
}

size_t TextEdit::calcCharPos(size_t row, size_t col) {
	printf("%zu %zu \n", row, col);
	int charPos = 0;
	for (int i = 0; i < row; i++) {
		charPos += TextLength(lines[i]) + 1;
	}

	std::string_view line = lines[row];
	charPos += (line.size() > col ? col : line.size());
	printf("%i \n", charPos);
	return charPos;
}

void TextEdit::leftMouseClicked() { setCursorFromMouse(); }

void TextEdit::leftMouseReleased() {}

void TextEdit::keyPressed(KeyboardKey key, KeyModifier mod, bool held) {
	bool val = held && debounce == 0;

	if (val) {
		auto &rend = render->as<TextEditRender>();
		float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
		float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

		Vector2 charMeasure = {0, 0};

		// left key
		if (key == KEY_LEFT) {
			if (charPos > 0) {
				int size = 0;
				char *ptr = text.data() + charPos;

				GetCodepointPrevious(ptr, &size);
				charPos -= size;

				cursorPos.column--;

				std::string sub = TextSubtext(text.c_str(), charPos, size);
				charMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);
				charMeasure.x *= -1;

				if (text.at(charPos) == '\n') {
					cursorPos.row--;
					cursorPos.column = TextLength(lines[cursorPos.row]);
					setCursorRect();
				}
			}
		}

		// right key
		if (key == KEY_RIGHT) {
			if (charPos < text.size()) {
				int size = 0;
				char *ptr = &text.at(charPos);

				GetCodepoint(ptr, &size);
				charPos += size;

				cursorPos.column++;

				std::string sub = TextSubtext(text.c_str(), charPos - size, size);
				charMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);

				if (text.at(charPos - 1) == '\n') {
					cursorPos.row++;
					cursorPos.column = 0;
					setCursorRect();
				}
			}
		}

		if (key == KEY_LEFT || key == KEY_RIGHT) {
			cursorRect.x += charMeasure.x;
		}

		if (key == KEY_UP) {
			if (cursorPos.row > 0) {
				cursorPos.row--;
				charPos = calcCharPos(cursorPos.row, cursorPos.column);
				setCursorRect();
			}
		}

		if (key == KEY_DOWN) {
			if (cursorPos.row < (rowCount - 1)) {
				cursorPos.row++;
				charPos = calcCharPos(cursorPos.row, cursorPos.column);
				setCursorRect();
			}
		}

		debounce = EDUI_TEXTEDIT_DEBOUNCE;
	}
}

void TextEdit::charEntered(int codepoint, std::string_view str) {
	printf("%i \n", codepoint);

	for (int i = 0; i < str.size(); i++) {
		text.insert(text.begin() + charPos + i, str.at(i));
	}

	unload();
	lines = LoadTextLines(this->text.c_str(), &rowCount);

	charPos += str.size();

	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 measure = MeasureTextEx(*rend.font, str.data(), totalFontSize, spacing);
	cursorRect.x += measure.x;

	Widget::charEntered(codepoint, str);
}
