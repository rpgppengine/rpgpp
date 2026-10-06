#ifndef _TREESITTER_CPP_TREE_H
#define _TREESITTER_CPP_TREE_H

#include <tree_sitter/api.h>

#include "tree_sitter/ts_node.hpp"

namespace ts {
struct Tree {
	TSTree *tree = nullptr;

	Tree(TSTree *tree) { this->tree = tree; }

	~Tree() {
		if (tree != nullptr) {
			ts_tree_delete(tree);
		}
	}

	ts::Node rootNode() { return ts_tree_root_node(tree); }
	void edit(const TSInputEdit *edit) { ts_tree_edit(tree, edit); }
};
}  // namespace ts

#endif
