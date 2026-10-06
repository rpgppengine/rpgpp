#ifndef _TREESITTER_CPP_QUERYCURSOR_H
#define _TREESITTER_CPP_QUERYCURSOR_H

#include <tree_sitter/api.h>

#include "tree_sitter/ts_node.hpp"
#include "tree_sitter/ts_query.hpp"

namespace ts {
struct QueryCursor {
	TSQueryCursor *cursor;

	QueryCursor() { cursor = ts_query_cursor_new(); }
	~QueryCursor() { ts_query_cursor_delete(cursor); }

	void exec(const ts::Query *query, ts::Node node) { ts_query_cursor_exec(cursor, query->query, node.node); }
	bool nextMatch(TSQueryMatch *match) { return ts_query_cursor_next_match(cursor, match); }
};
}  // namespace ts

#endif
