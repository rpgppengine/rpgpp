#include "edui/codeEdit.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "edui/gui.hpp"
#include "edui/textEdit.hpp"
#include "raylib.h"
#include "tree_sitter/api.h"
#include "tree_sitter/tree-sitter-lua.h"
#include "tree_sitter/ts_tree_cursor.hpp"

using namespace edui;

/// Taken from raylib
void DrawTextCode(Font font, const char *text, Vector2 position, float fontSize, float spacing,
				  std::vector<Color> &tint) {
	float textLineSpacing = 2;

	if (font.texture.id == 0) font = GetFontDefault();	// Security check in case of not valid font

	int size = TextLength(text);				   // Total size in bytes of the text, scanned by codepoints in loop

	float textOffsetY = 0;						   // Offset between lines (on linebreak '\n')
	float textOffsetX = 0.0f;					   // Offset X to next character to draw

	float scaleFactor = fontSize / font.baseSize;  // Character quad scaling factor

	int codepointCounter = 0;
	for (int i = 0; i < size;) {
		// Get next codepoint from byte string and glyph index in font
		int codepointByteCount = 0;
		int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
		int index = GetGlyphIndex(font, codepoint);

		if (codepoint == '\n') {
			// NOTE: Line spacing is a global variable, use SetTextLineSpacing() to setup
			textOffsetY += (fontSize + textLineSpacing);
			textOffsetX = 0.0f;
		} else {
			if ((codepoint != ' ') && (codepoint != '\t')) {
				Color pointTint = tint[codepointCounter];
				DrawTextCodepoint(font, codepoint, Vector2{position.x + textOffsetX, position.y + textOffsetY},
								  fontSize, pointTint);
			}

			if (font.glyphs[index].advanceX == 0)
				textOffsetX += (font.recs[index].width * scaleFactor + spacing);
			else
				textOffsetX += ((float)font.glyphs[index].advanceX * scaleFactor + spacing);
		}

		i += codepointByteCount;  // Move text bytes counter to next codepoint
		codepointCounter++;
	}
}
////

CodeEdit::CodeEdit() : TextEdit() {
	render = std::make_unique<edui::CodeEditRender>();
	render->padding = 2;

	tsParser.setLanguage(tree_sitter_lua());
	tsTree = nullptr;
}

void CodeEdit::applyTheme(const mINI::INIStructure &struc) {
	TextEdit::applyTheme(struc);
	highlighter.colors.applyTheme(struc);

	if (tsTree.tree != nullptr) {
		auto node = tsTree.rootNode();
		traverse(node);
	}
}

void CodeEdit::setText(const std::string &newText) {
	TextEdit::setText(newText);

	int count = GetCodepointCount(newText.c_str());
	codepointColors.assign(count, BLUE);

	tsTree.destroy();
	tsTree = tsParser.parseString(nullptr, newText);

	auto node = tsTree.rootNode();
	traverse(node);
}

TSInputEdit CodeEdit::beginTreeEdit(size_t *oldSize) {
	*oldSize = text.size();

	size_t startByte = charPos;
	TSPoint startPoint = {cursorPos.row, cursorPos.column};

	size_t oldEndByte = charPos;
	TSPoint oldEndPoint = {cursorPos.row, cursorPos.column};

	if (hasSelection()) {
		auto range = normalizeSelection();

		startByte = calcCharPos(range.start.row, range.start.column);
		startPoint = {range.start.row, range.start.column};
		oldEndPoint = {range.end.row, range.end.column};
	}

	TSInputEdit res;
	res.start_byte = startByte;
	res.start_point = startPoint;
	res.old_end_byte = oldEndByte;
	res.old_end_point = oldEndPoint;
	return res;
}

void CodeEdit::endTreeEdit(TSInputEdit &edit, size_t *oldSize) {
	size_t newSize = text.size();
	size_t added = newSize - *oldSize;

	size_t newEndByte = charPos;
	TSPoint newEndPoint = {cursorPos.row, cursorPos.column};

	edit.new_end_byte = newEndByte;
	edit.new_end_point = newEndPoint;

	tsTree.edit(&edit);
	tsTree = tsParser.parseString(&tsTree, text);

	auto &rend = render->as<TextEditRender>();
	if (newSize > *oldSize) {
		codepointColors.insert(codepointColors.cend(), added, rend.textColor);
	}

	auto root = tsTree.rootNode();
	traverse(root);
}

void CodeEdit::traverse(ts::Node &node) {
	auto &rend = render->as<CodeEditRender>();

	ts::TreeCursor cursor(node);

	if (cursor.gotoFirstChild()) {
		do {
			auto current = cursor.currentNode();
			if (current.isNamed()) {
				auto nodeColor = highlighter.getColor(current.type(), rend.textColor);
				for (int i = current.startByte(); i < current.endByte(); i++) {
					codepointColors[i] = nodeColor;
				}

				traverse(current);
			}
		} while (cursor.gotoNextSibling());
	}
}

void CodeEdit::drawText(Vector2 textBegin) {
	auto &rend = render->as<TextEditRender>();

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	DrawTextCode(*rend.font, text.c_str(), textBegin, totalFontSize, spacing, codepointColors);
}

void CodeEdit::handleEnterTab(KeyboardKey key) {
	size_t oldSize = 0;
	auto edit = beginTreeEdit(&oldSize);

	TextEdit::handleEnterTab(key);

	endTreeEdit(edit, &oldSize);
}

void CodeEdit::handleSelectionKeys(KeyboardKey key, KeyModifier mod) {
	size_t oldSize = 0;
	auto edit = beginTreeEdit(&oldSize);

	TextEdit::handleSelectionKeys(key, mod);

	endTreeEdit(edit, &oldSize);
}

void CodeEdit::charEntered(int codepoint, std::string_view str) {
	size_t oldSize = 0;
	auto edit = beginTreeEdit(&oldSize);

	TextEdit::charEntered(codepoint, str);

	endTreeEdit(edit, &oldSize);
}
