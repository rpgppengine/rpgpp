#ifndef _TREESITTER_CPP_TREE_H
#define _TREESITTER_CPP_TREE_H

#include <tree_sitter/api.h>

#include "tree_sitter/ts_node.hpp"

namespace ts {
struct Tree {
	TSTree *tree = nullptr;

	Tree() { this->tree = nullptr; }
	Tree(TSTree *tree) { this->tree = tree; }

	Tree(const Tree &newTree) {
		if (newTree.tree != nullptr) {
			this->tree = ts_tree_copy(newTree.tree);
		}
	}

	void operator=(Tree other) {
		if (other.tree != nullptr) {
			this->tree = ts_tree_copy(other.tree);
			other.tree = nullptr;
		}
	}

	~Tree() { destroy(); }

	void destroy() {
		if (tree != nullptr) {
			ts_tree_delete(tree);
		}
	}

	ts::Node rootNode() { return ts_tree_root_node(tree); }
	void edit(const TSInputEdit *edit) { ts_tree_edit(tree, edit); }
	ts::Tree shallowCopy() { return ts_tree_copy(tree); }
};
}  // namespace ts

#endif
