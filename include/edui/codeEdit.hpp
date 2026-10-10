#ifndef _EDUI_CODEEDIT_H
#define _EDUI_CODEEDIT_H

#include <vector>

#include "edui/textEdit.hpp"
#include "raylib.h"
#include "tree_sitter/ts_node.hpp"
#include "tree_sitter/ts_parser.hpp"
#include "tree_sitter/ts_tree.hpp"
namespace edui {
struct CodeEditRender : public TextEditRender {};

struct CodeEdit : public TextEdit {
	ts::Parser tsParser;
	ts::Tree tsTree;

	CodeEdit();

	void setText(const std::string &newText) override;

	void charEntered(int codepoint, std::string_view str) override;

protected:
	void drawText(Vector2 textBegin) override;
	void handleEnterTab(KeyboardKey key) override;

private:
	std::vector<Color> codepointColors = {};
	void traverse(ts::Node &node);
};
}  // namespace edui

#endif
