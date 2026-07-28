#include "edui/textBox.hpp"
#include <cstddef>
#include <cstdio>
#include <memory>
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

TextBox::TextBox() {
	render = std::make_unique<TextBoxRender>();
	render->padding = 2.0f;
	focusable = true;
}

void TextBox::update() {
	if (debounce > 0) {
		debounce--;
	}
}

void TextBox::draw() {
	auto &rend = render->as<TextBoxRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();
	float totalFontSize = rend.font->baseSize * rend.fontSize;

	BeginScissorMode(paddingRect.x, rect.y, paddingRect.width, rect.height);

	DrawTextEx(*rend.font, text.c_str(), {paddingRect.x - scroll, paddingRect.y}, totalFontSize, rend.spacing, rend.textColor);

	EndScissorMode();

	if (isFocused) {
		DrawRectangleRec(cursorRect, BLACK);
	}
}

void TextBox::setText(const std::string& text) {
	this->text = text;
}

void TextBox::focused() {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return;
	if (text.empty()) return;

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	auto mousePos = GetMousePosition();

	auto paddingRect = getPaddingRect();
	Vector2 offset = Vector2Subtract(mousePos, {paddingRect.x + scroll, paddingRect.y});
	offset = Vector2Add(offset, {paddingRect.x, paddingRect.y});
	float offsetVal = (offset.x + scroll) - paddingRect.x;

	auto end = GetCodepointCount(text.c_str());
	char* ptr = text.data();

	std::string clonedStr = text;
	char* b = clonedStr.data();

	int codepointSize = 0;
	int len = 0;
	GetCodepoint(ptr, &codepointSize);
	len += codepointSize;

	std::string subStr = TextSubtext(text.c_str(), 0, len);
	auto measure = MeasureTextEx(*rend.font, subStr.c_str(), totalFontSize, rend.spacing);
	measure.x -= paddingRect.x;

	for (int i = 0; i < end; i++) {
		if (measure.x - scroll > offsetVal) {
			cursorIndex = len - 1;
			calcCursorRect();
			break;
		}

		subStr = TextSubtext(text.c_str(), 0, len);
		measure = MeasureTextEx(*rend.font, subStr.c_str(), totalFontSize, rend.spacing);

		GetCodepointNext(ptr, &codepointSize);
		len += codepointSize;
		ptr += codepointSize;
	}

	Widget::focused();
}

void TextBox::unfocused() {
	Widget::unfocused();
}

void TextBox::calcCursorRect() {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return;
	float totalFontSize = rend.font->baseSize * rend.fontSize;
	auto paddingRect = getPaddingRect();

	std::string subStr = TextSubtext(text.c_str(), 0, cursorIndex);
	auto measure = MeasureTextEx(*rend.font, subStr.c_str(), totalFontSize, rend.spacing);
	cursorRect = {measure.x - scroll + paddingRect.x, paddingRect.y, CursorWidth, paddingRect.height};

	if (cursorRect.x + cursorRect.width > paddingRect.x + paddingRect.width) {
		float diff = (cursorRect.x + cursorRect.width) - (paddingRect.x + paddingRect.width);
		cursorRect.x -= diff;
		scroll += diff;
	}
	if (cursorRect.x < paddingRect.x) {
		float diff = (paddingRect.x - cursorRect.x);
		cursorRect.x += diff;
		scroll -= diff;
	}
}

void TextBox::keyPressed(KeyboardKey key, bool held) {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return;
	if (text.empty()) return;

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	if (debounce == 0) {
		if (key == KEY_LEFT) {
			if (cursorIndex > 0) {
				int size = 0;
				char* ptr = text.data() + cursorIndex;

				GetCodepointPrevious(ptr, &size);
				cursorIndex -= size;

				calcCursorRect();
			}
		}
		if (key == KEY_RIGHT) {
			if (cursorIndex < text.size()) {
				int size = 0;
				char* ptr = &text.at(cursorIndex);

				GetCodepoint(ptr, &size);
				cursorIndex += size;

				calcCursorRect();
			}
		}
		if (key == KEY_BACKSPACE) {
			if (cursorIndex > 0) {
				bool lastChar = false;
				if (cursorIndex == text.size()) {
					lastChar = true;
				}

				int size = 0;
				char* ptr = text.data() + cursorIndex;

				GetCodepointPrevious(ptr, &size);
				cursorIndex -= size;
				ptr -= size;

				text = text.erase(cursorIndex, size);
				calcCursorRect();

				if (lastChar) {
					auto paddingRect = getPaddingRect();
					float diff = (paddingRect.x + paddingRect.width) - (cursorRect.x + cursorRect.width);
					cursorRect.x += diff;
					scroll -= diff;
				}
			}
		}

		debounce = DebounceFrames;
	}

	Widget::keyPressed(key, held);
}

void TextBox::charEntered(char c) {
	text.insert(text.begin() + cursorIndex, c);
	cursorIndex++;

	calcCursorRect();

	Widget::charEntered(c);
}

void TextBox::mouseEntered() {
	SetMouseCursor(MOUSE_CURSOR_IBEAM);
	Widget::mouseEntered();
}

void TextBox::mouseLeft() {
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	Widget::mouseLeft();
}
