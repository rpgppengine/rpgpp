#ifndef _EDUI_CODEEDIT_H
#define _EDUI_CODEEDIT_H

#include <cstddef>
#include <vector>

#include "edui/syntaxHighlighter.hpp"
#include "edui/textEdit.hpp"
#include "raylib.h"
#include "tree_sitter/api.h"
#include "tree_sitter/ts_node.hpp"
#include "tree_sitter/ts_parser.hpp"
#include "tree_sitter/ts_tree.hpp"
namespace edui {
struct CodeEditRender : public TextEditRender {};

struct CodeEdit : public TextEdit {
	ts::Parser tsParser;
	ts::Tree tsTree;

	CodeEdit();
	void applyTheme(const mINI::INIStructure &struc) override;

	void setText(const std::string &newText) override;

	void charEntered(int codepoint, std::string_view str) override;

protected:
	void drawText(Vector2 textBegin) override;
	void handleEnterTab(KeyboardKey key) override;
	void handleSelectionKeys(KeyboardKey key, KeyModifier mod) override;

private:
	SyntaxHighlighter highlighter;
	std::vector<Color> codepointColors = {};
	void traverse(ts::Node &node);
	TSInputEdit beginTreeEdit(size_t *oldSize);
	void endTreeEdit(TSInputEdit &edit, size_t *newSize);
};
}  // namespace edui

#endif
