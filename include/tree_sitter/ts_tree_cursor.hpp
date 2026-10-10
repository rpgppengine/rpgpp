#ifndef _TREESITTER_CPP_CURSOR_H
#define _TREESITTER_CPP_CURSOR_H

#include <tree_sitter/api.h>

#include <cstdint>

#include "tree_sitter/ts_node.hpp"

namespace ts {
struct TreeCursor {
	TSTreeCursor cursor;

	TreeCursor(ts::Node node) { cursor = ts_tree_cursor_new(node.node); }
	~TreeCursor() { ts_tree_cursor_delete(&cursor); }

	bool gotoFirstChild() { return ts_tree_cursor_goto_first_child(&cursor); }
	bool gotoLastChild() { return ts_tree_cursor_goto_last_child(&cursor); }
	bool gotoNextSibling() { return ts_tree_cursor_goto_next_sibling(&cursor); }
	ts::Node currentNode() { return ts_tree_cursor_current_node(&cursor); }

	int32_t gotoFirstChildForByte(uint32_t byte) { return ts_tree_cursor_goto_first_child_for_byte(&cursor, byte); }
};
}  // namespace ts

#endif
