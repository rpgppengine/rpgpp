#ifndef _TREESITTER_CPP_NODE_H
#define _TREESITTER_CPP_NODE_H

#include <tree_sitter/api.h>

#include <cstdint>
#include <string>

namespace ts {
struct Node {
	TSNode node;

	Node(TSNode node) { this->node = node; }

	~Node() {}

	std::string type() { return ts_node_type(node); }
	uint32_t namedChildCount() { return ts_node_named_child_count(node); }
	ts::Node namedChild(uint32_t i) { return ts_node_named_child(node, i); }
	TSPoint startPoint() { return ts_node_start_point(node); }
	bool isNamed() { return ts_node_is_named(node); }
	std::string string() { return ts_node_string(node); }
};
}  // namespace ts

#endif
