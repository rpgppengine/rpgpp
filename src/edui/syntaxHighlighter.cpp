#include "edui/syntaxHighlighter.hpp"

#include <vector>

#include "edui/helper.hpp"

static const std::vector<std::string> stringTypes = {"string", "string_content"};
static const std::vector<std::string> commentTypes = {"comment", "comment_content"};
static const std::vector<std::string> keywordTypes = {"function_declaration",
													  "variable_declaration",
													  "for_statement",
													  "if_statement",
													  "else_statement",
													  "do_statement",
													  "label_statement",
													  "repeat_statement",
													  "while_statement",
													  "goto_statement",
													  "break_statement",
													  "return_statement",
													  "field",
													  "for_generic_clause"};
static const std::vector<std::string> operatorTypes = {
	"binary_expression", "unary_expression", "parenthesized_expression", "table_constructor", "assignment_statement"};
static const std::vector<std::string> literalTypes = {"number", "nil", "true", "false"};

Color edui::SyntaxHighlighter::getColor(const std::string &type, Color &defaultColor) {
	if (stringInArr(type, keywordTypes)) {
		return colors.keyword;
	}
	if (stringInArr(type, operatorTypes)) {
		return colors.operatorC;
	}

	if (type == "identifier") {
		return colors.identifier;
	}
	if (stringInArr(type, stringTypes)) {
		return colors.string;
	}
	if (stringInArr(type, commentTypes)) {
		return colors.comment;
	}
	if (stringInArr(type, literalTypes)) {
		return colors.number;
	}

	return defaultColor;
}
