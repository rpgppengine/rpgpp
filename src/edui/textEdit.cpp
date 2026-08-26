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
	reloadLines();
}

void TextEdit::update() {
	if (debounce > 0) {
		debounce--;
	}
}

void TextEdit::draw() {
	auto &rend = render->as<TextEditRender>();

	DrawRectangleRec(rect, rend.bgColor);

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	scissorContentRect = getPaddingRect();
	Vector2 textBegin = {rect.x + rend.padding - scissorX, rect.y + rend.padding - scissorY};
	BeginScissorMode(scissorContentRect.x, scissorContentRect.y, scissorContentRect.width, scissorContentRect.height);

	DrawTextEx(*rend.font, text.c_str(), textBegin, totalFontSize, spacing, rend.borderColor);

	if (isFocused) {
		drawCursor();
	}

	EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void TextEdit::leftMouseClicked() { setCursorFromMouse(); }

void TextEdit::leftMouseReleased() {}

void TextEdit::mouseEntered() {
	SetMouseCursor(MOUSE_CURSOR_IBEAM);
	Widget::mouseEntered();
}

void TextEdit::mouseLeft() {
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	Widget::mouseLeft();
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

	mousePos.y += scissorY;
	mousePos.x += scissorX;

	int row = static_cast<int>((mousePos.y / (totalFontSize + 2)));
	if (row >= rowCount) row = rowCount - 1;
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

	this->charPos = calcCharPos(row, col);
	setCursorRect();
}

void TextEdit::setCursorRect() {
	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	std::string_view line = lines[cursorPos.row];

	std::string sub = TextSubtext(line.data(), 0, cursorPos.column);
	Vector2 subMeasure = MeasureTextEx(*rend.font, sub.c_str(), totalFontSize, spacing);
	cursorRect.x = rect.x + rend.padding + subMeasure.x - scissorX;
	cursorRect.y = rect.y + rend.padding + ((totalFontSize + 2) * cursorPos.row) - scissorY;
	cursorRect.height = totalFontSize;

	setOffset();
}

size_t TextEdit::calcCharPos(size_t row, size_t col) {
	int charPos = 0;
	for (int i = 0; i < row; i++) {
		charPos += TextLength(lines[i]) + 1;
	}

	std::string_view line = lines[row];
	int count = GetCodepointCount(line.data());

	charPos += (count > col ? col : count);
	return charPos;
}

void TextEdit::reloadLines() {
	unload();
	lines = LoadTextLines(this->text.c_str(), &rowCount);

	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	scrollMax = (rowCount * (totalFontSize + 2));
	if (scrollMax > scissorContentRect.height) {
		overflownY = true;
		scrollMax -= scissorContentRect.height;
	} else {
		overflownY = false;
	}
}

void TextEdit::setOffset() {
	auto paddingRect = scissorContentRect;

	// x
	if ((cursorRect.x + cursorRect.width) > (paddingRect.x + paddingRect.width)) {
		float diff = (cursorRect.x + cursorRect.width) - (paddingRect.x + paddingRect.width);
		scissorX += diff;
		cursorRect.x -= diff;
	}
	if (cursorRect.x < paddingRect.x) {
		float diff = paddingRect.x - cursorRect.x;
		scissorX -= diff;
		cursorRect.x += diff;
	}

	// y
	if ((cursorRect.y + cursorRect.height) > (paddingRect.y + paddingRect.height)) {
		float diff = (cursorRect.y + cursorRect.height) - (paddingRect.y + paddingRect.height);
		scissorY += diff;
		cursorRect.y -= diff;
	}
	if (cursorRect.y < paddingRect.y) {
		float diff = paddingRect.y - cursorRect.y;
		scissorY -= diff;
		cursorRect.y += diff;
	}
}

void TextEdit::handleArrowKeys(KeyboardKey key) {
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

	// up
	if (key == KEY_UP) {
		if (cursorPos.row > 0) {
			cursorPos.row--;
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}
	}

	// down
	if (key == KEY_DOWN) {
		if (cursorPos.row < (rowCount - 1)) {
			cursorPos.row++;
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}
	}

	setOffset();
}

void TextEdit::handleDeletionKeys(KeyboardKey key) {
	if (key == KEY_BACKSPACE) {
		if (charPos > 0) {
			int size = 0;
			char *ptr = text.data() + charPos;
			GetCodepointPrevious(ptr, &size);

			charPos -= size;
			cursorPos.column--;

			if (text.at(charPos) == '\n') {
				cursorPos.row--;
				std::string_view line = lines[cursorPos.row];
				cursorPos.column = line.size();
			}

			text = text.erase(charPos, size);
			reloadLines();
			setCursorRect();
		}
	}

	if (key == KEY_DELETE) {
		if (charPos > 0) {
			int size = 0;
			char *ptr = text.data() + charPos;
			GetCodepoint(ptr, &size);

			text = text.erase(charPos, size);
			reloadLines();
		}
	}
}

void TextEdit::handleEnterTab(KeyboardKey key) {
	if (key == KEY_ENTER) {
		int size = 0;
		char *ptr = text.data() + charPos;
		GetCodepoint(ptr, &size);
		text.insert(text.begin() + charPos, '\n');

		cursorPos.row++;
		cursorPos.column = 0;
		charPos++;

		reloadLines();
		setCursorRect();
	}

	if (key == KEY_TAB) {
		text.insert(text.begin() + charPos, '\t');
		cursorPos.column++;
		charPos++;

		reloadLines();
		setCursorRect();
	}
}

void TextEdit::keyPressed(KeyboardKey key, KeyModifier mod, bool held) {
	if (!held) {
		// home
		if (key == KEY_HOME || (key == KEY_KP_7 && mod.numlock)) {
			cursorPos.column = 0;
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}

		// end
		if (key == KEY_END || (key == KEY_KP_1 && mod.numlock)) {
			std::string_view line = lines[cursorPos.row];
			cursorPos.column = line.size();
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}
	}

	bool val = held && debounce == 0;

	if (val) {
		handleArrowKeys(key);
		handleDeletionKeys(key);
		handleEnterTab(key);

		debounce = EDUI_TEXTEDIT_DEBOUNCE;
	}
}

void TextEdit::charEntered(int codepoint, std::string_view str) {
	for (int i = 0; i < str.size(); i++) {
		text.insert(text.begin() + charPos + i, str.at(i));
	}

	reloadLines();

	charPos += str.size();
	cursorPos.column++;

	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 measure = MeasureTextEx(*rend.font, str.data(), totalFontSize, spacing);
	cursorRect.x += measure.x;

	setOffset();

	Widget::charEntered(codepoint, str);
}

void TextEdit::scrolled(float mouseWheel) {
	if (!overflownY) return;
	if (mouseWheel == 0) return;

	float added = (mouseWheel * EDUI_TEXTEDIT_SCROLLSCPEED);

	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;

	if ((scissorY - added) < 0.0f) {
		scissorY = 0;
		cursorRect.y = rect.y + rend.padding + ((totalFontSize + 2) * cursorPos.row) - scissorY;
		return;
	}

	if ((scissorY - added) > scrollMax) {
		scissorY = scrollMax;
		cursorRect.y = rect.y + rend.padding + ((totalFontSize + 2) * cursorPos.row) - scissorY;
		return;
	}

	scissorY -= added;
	cursorRect.y += added;
}
