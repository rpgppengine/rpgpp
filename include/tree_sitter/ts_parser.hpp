#ifndef _TREESITTER_CPP_PARSER_H
#define _TREESITTER_CPP_PARSER_H

#include <tree_sitter/api.h>

#include <string>

#include "tree_sitter/ts_tree.hpp"

namespace ts {
struct Parser {
	TSParser *parser = nullptr;

	Parser() { parser = ts_parser_new(); }
	Parser(TSParser *parser) { this->parser = parser; }

	~Parser() { ts_parser_delete(parser); }

	void setLanguage(const TSLanguage *language) { ts_parser_set_language(parser, language); }
	const TSLanguage *getLanguage() { return ts_parser_language(parser); }

	ts::Tree parseString(ts::Tree *tree, const std::string &str) {
		if (tree == nullptr) {
			return ts_parser_parse_string(parser, nullptr, str.data(), str.size());
		} else {
			return ts_parser_parse_string(parser, tree->tree, str.data(), str.size());
		}
	}
	ts::Tree parse(const TSTree *old_tree, TSInput input) { return ts_parser_parse(parser, old_tree, input); }
};
}  // namespace ts

#endif
