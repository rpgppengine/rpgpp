#include "edui/textEdit.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string_view>
#include <utility>

#include "edui/gui.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

edui::CharsRange CodepointsToCharsRange(std::string_view str, size_t start, size_t end) {
	edui::CharsRange range;

	const char *ptr = str.data();
	for (int i = 0; i < start; i++) {
		int codepointSize = 0;
		GetCodepoint(ptr, &codepointSize);
		ptr += codepointSize;
		range.start += codepointSize;
	}

	for (int i = start; i < end; i++) {
		int codepointSize = 0;
		GetCodepoint(ptr, &codepointSize);
		ptr += codepointSize;
		range.end += codepointSize;
	}
	range.end += range.start;

	return range;
}

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

	if (hasSelection()) {
		drawSelection();
	}

	if (isFocused) {
		drawCursor();
	}

	EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void TextEdit::leftMouseClicked() {
	setCursorFromMouse();
	mouseHeld = true;
}

void TextEdit::leftMouseReleased() { mouseHeld = false; }

void TextEdit::mouseEntered() {
	SetMouseCursor(MOUSE_CURSOR_IBEAM);
	Widget::mouseEntered();
}

void TextEdit::mouseLeft() {
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	Widget::mouseLeft();
}

void TextEdit::mouseMoved(Vector2 mousePos, Vector2 relative) {
	if (mouseHeld) {
		auto pos = getPositionFromMouse();
		auto range = CodepointsToCharsRange(lines[pos.row], 0, pos.column);
		this->selectEnd = pos;
		this->cursorPos = pos;
		setCursorRect();
		calcCharPos(cursorPos.row, cursorPos.column);
	}
}

void TextEdit::drawCursor() {
	auto &rend = render->as<TextEditRender>();
	DrawRectangleRec(cursorRect, rend.borderColor);
}

void TextEdit::setCursorFromMouse() {
	this->cursorPos = getPositionFromMouse();
	this->selectStart = cursorPos;
	this->selectEnd = cursorPos;
	this->charPos = calcCharPos(cursorPos.row, cursorPos.column);
	setCursorRect();
}

CursorPosition TextEdit::getPositionFromMouse() {
	CursorPosition cursorPos = {0, 0};

	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 mousePos = GetMousePosition();
	mousePos = Vector2Subtract(mousePos, {rect.x + rend.padding, rect.y + rend.padding});

	if (mousePos.x < 0 || mousePos.y < 0) return CursorPosition{0, 0};

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

	return cursorPos;
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

bool TextEdit::hasSelection() { return (selectStart.row != selectEnd.row) || (selectStart.column != selectEnd.column); }

PosRange TextEdit::normalizeSelection() {
	PosRange result = {selectStart, selectEnd};

	if (selectStart.row == selectEnd.row) {
		if (selectEnd.column < selectStart.column) std::swap(result.start, result.end);
	} else if (selectEnd.row < selectStart.row) {
		std::swap(result.start, result.end);
	}

	return result;
}

void TextEdit::drawSelectionLine(PosRange range) {
	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	auto bytes = CodepointsToCharsRange(lines[range.start.row], range.start.column, range.end.column);

	std::string subA = TextSubtext(lines[range.start.row], 0, bytes.start);
	Vector2 measureA = MeasureTextEx(*rend.font, subA.c_str(), totalFontSize, spacing);

	std::string subB = TextSubtext(lines[range.start.row], bytes.start, bytes.end - bytes.start);
	Vector2 measureB = MeasureTextEx(*rend.font, subB.c_str(), totalFontSize, spacing);

	Rectangle selectionRect = {(rect.x + rend.padding + measureA.x - scissorX),
							   (rect.y + rend.padding + ((totalFontSize + 2) * range.start.row) - scissorY),
							   (measureB.x), (totalFontSize + 2)};
	DrawRectangleRec(selectionRect, Fade(GRAY, 0.5f));
}

void TextEdit::drawSelection() {
	auto &rend = render->as<TextEditRender>();
	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	auto range = normalizeSelection();

	size_t firstLineEnd =
		(range.start.row == range.end.row) ? range.end.column : GetCodepointCount(lines[range.start.row]);
	PosRange firstRange = {range.start, {range.start.row, static_cast<uint16_t>(firstLineEnd)}};
	drawSelectionLine(firstRange);

	for (size_t iRow = (range.start.row + 1); iRow < (range.end.row); iRow++) {
		PosRange iRange = {{static_cast<uint16_t>(iRow), 0},
						   {static_cast<uint16_t>(iRow), static_cast<uint16_t>(GetCodepointCount(lines[iRow]))}};
		drawSelectionLine(iRange);
	}

	if (range.start.row != range.end.row) {
		PosRange lastRange = {{range.end.row, 0}, range.end};
		drawSelectionLine(lastRange);
	}
}

void TextEdit::resetSelection() {
	selectStart = {0, 0};
	selectEnd = {0, 0};
}

void TextEdit::eraseSelection() {
	auto selectNormalized = normalizeSelection();

	size_t selectStartByte = calcCharPos(selectNormalized.start.row, selectNormalized.start.column);
	size_t selectEndByte = calcCharPos(selectNormalized.end.row, selectNormalized.end.column);

	charPos = selectStartByte;
	cursorPos = selectNormalized.start;

	text = text.erase(selectStartByte, selectEndByte - selectStartByte);

	reloadLines();
	setCursorRect();

	resetSelection();
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

	if (scissorY > scrollMax) {
		float diff = (paddingRect.y + paddingRect.height) - (cursorRect.y + cursorRect.height);
		scissorY -= diff;
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
		resetSelection();
	}

	// up
	if (key == KEY_UP) {
		if (cursorPos.row > 0) {
			cursorPos.row--;
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}
		resetSelection();
	}

	// down
	if (key == KEY_DOWN) {
		if (cursorPos.row < (rowCount - 1)) {
			cursorPos.row++;
			charPos = calcCharPos(cursorPos.row, cursorPos.column);
			setCursorRect();
		}
		resetSelection();
	}

	setOffset();
}

void TextEdit::handleDeletionKeys(KeyboardKey key) {
	if (key == KEY_BACKSPACE) {
		if (charPos > 0) {
			if (hasSelection()) {
				eraseSelection();
				return;
			}

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
			if (hasSelection()) {
				eraseSelection();
				return;
			}

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
		if (hasSelection()) {
			eraseSelection();
		}

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
		if (hasSelection()) {
			eraseSelection();
		}

		text.insert(text.begin() + charPos, '\t');
		cursorPos.column++;
		charPos++;

		reloadLines();
		setCursorRect();
	}
}

void TextEdit::handleSelectionKeys(KeyboardKey key, KeyModifier mod) {
	auto selectNormalized = normalizeSelection();

	size_t selectStartByte = calcCharPos(selectNormalized.start.row, selectNormalized.start.column);
	size_t selectEndByte = calcCharPos(selectNormalized.end.row, selectNormalized.end.column);

	std::string selectionText = TextSubtext(text.c_str(), selectStartByte, selectEndByte - selectStartByte);

	if (key == KEY_C && mod.ctrl) {
		if (hasSelection()) {
			SetClipboardText(selectionText.c_str());
		}
	}
	if (key == KEY_V && mod.ctrl) {
		if (GetClipboardText() == NULL) return;

		if (hasSelection()) {
			eraseSelection();
		}

		std::string clipboard = GetClipboardText();
		text = text.insert(charPos, clipboard);

		auto oldCharPos = charPos;
		charPos += clipboard.size();

		for (size_t i = oldCharPos; i < charPos; i++) {
			cursorPos.column++;
			if (text.at(i) == '\n') {
				cursorPos.column = 0;
				cursorPos.row++;
			}
		}

		reloadLines();
		setCursorRect();
	}
	if (key == KEY_X && mod.ctrl) {
		if (hasSelection()) {
			SetClipboardText(selectionText.c_str());

			eraseSelection();
		}
	}
	if (key == KEY_A && mod.ctrl) {
		selectStart.row = 0;
		selectStart.column = 0;

		selectEnd.row = rowCount - 1;
		selectEnd.column = GetCodepointCount(lines[rowCount - 1]);

		cursorPos = selectEnd;
		setCursorRect();
		charPos = calcCharPos(cursorPos.row, cursorPos.column);
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

		handleSelectionKeys(key, mod);
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
