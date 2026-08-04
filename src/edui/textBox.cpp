#include "edui/textBox.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string_view>

#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

TextBox::TextBox() {
	render = std::make_unique<TextBoxRender>();
	render->padding = 2.0f;
	render->focusBgColor = RAYWHITE;
	focusable = true;
}

void TextBox::update() {
	if (debounce > 0) {
		debounce--;
	}

	if (mouseBtnHeld) {
		int calcCursor = this->calcCursor();
		if (calcCursor == -1) {
			return;
		}

		selectEnd = calcCursor;
		cursorIndex = selectEnd;
		calcCursorRect();
	}
}

SelectionResult TextBox::calcSelection() {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return {0, 0, true};
	if (text.empty()) return {0, 0, true};

	int tempStart = selectStart;
	int tempEnd = selectEnd;

	int calcCursor = this->calcCursor();
	if (calcCursor == -1) {
		Widget::leftMouseReleased();
		return {0, 0, true};
	}

	tempEnd = calcCursor;

	int a = std::max(tempStart, tempEnd);
	int b = std::min(tempStart, tempEnd);

	tempStart = b;
	tempEnd = a;

	return {tempStart, tempEnd, false};
}

SelectionResult TextBox::getSelectionInfo() {
	SelectionResult selection;

	int a = std::max(selectStart, selectEnd);
	int b = std::min(selectStart, selectEnd);

	selection.start = b;
	selection.end = a;

	return selection;
}

void TextBox::drawSelection() {
	if (selectStart != selectEnd) {
		auto &rend = render->as<TextBoxRender>();

		if (rend.font == nullptr) return;

		auto selection = calcSelection();
		if (!mouseBtnHeld) {
			selection = getSelectionInfo();
		}

		auto paddingRect = getPaddingRect();
		float totalFontSize = rend.font->baseSize * rend.fontSize;

		std::string stringA = TextSubtext(text.c_str(), 0, selection.start);
		Vector2 measureA = MeasureTextEx(*rend.font, stringA.c_str(), totalFontSize, rend.spacing);

		std::string stringB = TextSubtext(text.c_str(), selection.start, selection.end - selection.start);
		Vector2 measureB = MeasureTextEx(*rend.font, stringB.c_str(), totalFontSize, rend.spacing);

		Rectangle selectionRect = {paddingRect.x + measureA.x - scroll, paddingRect.y, measureB.x, paddingRect.height};

		DrawRectangleRec(selectionRect, Fade(GRAY, 0.3f));
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

	DrawTextEx(*rend.font, text.c_str(), {paddingRect.x - scroll, paddingRect.y}, totalFontSize, rend.spacing,
			   rend.textColor);
	drawSelection();

	EndScissorMode();

	if (isFocused) {
		DrawRectangleRec(cursorRect, BLACK);
	}
}

void TextBox::setText(const std::string &text) { this->text = text; }

void TextBox::focused() {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return;
	if (text.empty()) return;

	int calcCursor = this->calcCursor();
	if (calcCursor == -1) return;
	this->cursorIndex = calcCursor;
	calcCursorRect();

	Widget::focused();
}

void TextBox::unfocused() {
	selectStart = selectEnd;
	Widget::unfocused();
}

int TextBox::calcCursor() {
	int res = -1;

	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return res;
	if (text.empty()) return res;

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	auto mousePos = GetMousePosition();

	auto paddingRect = getPaddingRect();
	Vector2 offset = Vector2Subtract(mousePos, {paddingRect.x + scroll, paddingRect.y});
	float offsetVal = offset.x + scroll;

	auto end = GetCodepointCount(text.c_str());
	char *ptr = text.data();

	std::string clonedStr = text;
	char *b = clonedStr.data();

	int codepointSize = 0;
	int len = 0;
	int firstChar = 1;
	GetCodepoint(ptr, &firstChar);

	std::string subStr = TextSubtext(text.c_str(), 0, len);
	auto measure = Vector2{offsetVal, 0};
	measure.x -= paddingRect.x;

	for (int i = 0; i <= end; i++) {
		if ((measure.x - scroll) > offsetVal) {
			break;
		}

		subStr = TextSubtext(text.c_str(), 0, len);
		measure = MeasureTextEx(*rend.font, subStr.c_str(), totalFontSize, rend.spacing);

		GetCodepoint(ptr, &codepointSize);
		len += codepointSize;
		ptr += codepointSize;
	}

	res = len - codepointSize;

	return res;
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

void TextBox::keyPressed(KeyboardKey key, KeyModifier mod, bool held) {
	auto &rend = render->as<TextBoxRender>();
	if (rend.font == nullptr) return;
	if (text.empty()) return;

	if (!held) {
		// combos
		if (key == KEY_C && mod.ctrl) {
			if (selectStart != selectEnd) {
				SelectionResult selection = getSelectionInfo();

				std::string selectionText = TextSubtext(text.c_str(), selection.start, selection.end - selection.start);
				SetClipboardText(selectionText.c_str());
			}
		}
		if (key == KEY_V && mod.ctrl) {
			auto clipboardRaw = GetClipboardText();
			if (clipboardRaw == NULL) return;
			std::string clipboard = clipboardRaw;
			if (clipboard.empty()) return;

			auto selection = getSelectionInfo();
			if (selection.start != selection.end) {
				text.erase(selection.start, selection.end - selection.start);
				cursorIndex = selection.start;
				selectStart = selectEnd;
			}

			text = text.insert(cursorIndex, clipboard);
			cursorIndex += clipboard.size();
			calcCursorRect();
		}
		if (key == KEY_A && mod.ctrl) {
			selectStart = 0;
			selectEnd = text.size();
			cursorIndex = selectEnd;
			calcCursorRect();
		}

		// home
		if (key == KEY_HOME || (key == KEY_KP_7 && mod.numlock)) {
			cursorIndex = 0;
			calcCursorRect();
		}

		// end
		if (key == KEY_END || (key == KEY_KP_1 && mod.numlock)) {
			cursorIndex = text.size();
			calcCursorRect();
		}
	}

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	bool val = held && debounce == 0;

	if (val) {
		if (key == KEY_LEFT) {
			if (cursorIndex > 0) {
				int size = 0;
				char *ptr = text.data() + cursorIndex;

				GetCodepointPrevious(ptr, &size);
				cursorIndex -= size;

				calcCursorRect();

				selectStart = selectEnd;
			}
		}
		if (key == KEY_RIGHT) {
			if (cursorIndex < text.size()) {
				int size = 0;
				char *ptr = &text.at(cursorIndex);

				GetCodepoint(ptr, &size);
				cursorIndex += size;

				calcCursorRect();

				selectStart = selectEnd;
			}
		}
		if (key == KEY_BACKSPACE) {
			if (selectStart != selectEnd) {
				auto selection = getSelectionInfo();
				text = text.erase(selection.start, selection.end - selection.start);
				cursorIndex = selection.start;
				calcCursorRect();
				selectStart = selectEnd;
				return;
			}
			if (cursorIndex > 0) {
				bool lastChar = false;
				if (cursorIndex == text.size()) {
					lastChar = true;
				}

				int size = 0;
				char *ptr = text.data() + cursorIndex;

				GetCodepointPrevious(ptr, &size);
				cursorIndex -= size;
				ptr -= size;

				text = text.erase(cursorIndex, size);
				calcCursorRect();

				auto paddingRect = getPaddingRect();

				if (lastChar) {
					auto &rend = render->as<TextBoxRender>();
					float totalFontSize = rend.font->baseSize * rend.fontSize;
					auto measure = MeasureTextEx(*rend.font, text.c_str(), totalFontSize, rend.spacing);
					if (measure.x >= paddingRect.x + paddingRect.width) {
						auto paddingRect = getPaddingRect();
						float diff = (paddingRect.x + paddingRect.width) - (cursorRect.x + cursorRect.width);
						cursorRect.x += diff;
						scroll -= diff;
					}
				}
			}
		}
		if (key == KEY_DELETE) {
			if (selectStart != selectEnd) {
				auto selection = getSelectionInfo();
				text = text.erase(selection.start, selection.end - selection.start);
				cursorIndex = selection.start;
				calcCursorRect();
				selectStart = selectEnd;
				return;
			}

			int size = 0;
			char *ptr = text.data() + cursorIndex;
			GetCodepoint(ptr, &size);

			text = text.erase(cursorIndex, size);
		}

		debounce = DebounceFrames;
	}

	Widget::keyPressed(key, mod, held);
}

void TextBox::leftMouseClicked() {
	mouseBtnHeld = true;
	int calcCursor = this->calcCursor();
	if (calcCursor == -1) {
		Widget::leftMouseClicked();
		return;
	}

	selectStart = calcCursor;
	selectEnd = calcCursor;
	Widget::leftMouseClicked();
}

void TextBox::leftMouseReleased() {
	if (!mouseIsInRect()) {
		mouseBtnHeld = false;
		return;
	}

	mouseBtnHeld = false;

	Widget::leftMouseReleased();
}

void TextBox::charEntered(int codepoint, std::string_view str) {
	for (int i = 0; i < str.size(); i++) {
		text.insert(text.begin() + cursorIndex + i, str.at(i));
	}
	cursorIndex += str.size();

	calcCursorRect();

	Widget::charEntered(codepoint, str);
}

void TextBox::mouseEntered() {
	SetMouseCursor(MOUSE_CURSOR_IBEAM);
	Widget::mouseEntered();
}

void TextBox::mouseLeft() {
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	Widget::mouseLeft();
}
