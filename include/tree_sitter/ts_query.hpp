#ifndef _TREESITTER_CPP_QUERY_H
#define _TREESITTER_CPP_QUERY_H

#include <tree_sitter/api.h>

#include <cstdint>
#include <string>

namespace ts {
struct Query {
	TSQuery *query;

	Query(const TSLanguage *language, const std::string &source, uint32_t *error_offset, TSQueryError *error_type) {
		query = ts_query_new(language, source.c_str(), source.size(), error_offset, error_type);
	}
	~Query() { ts_query_delete(query); }
};
}  // namespace ts

#endif
