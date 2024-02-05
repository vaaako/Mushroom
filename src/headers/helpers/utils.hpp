#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include "../types/node.hpp"
#include "../types/token.hpp"

// const std::array<std::string, 15> tokenTypes = { "EndOfFile", "Number", "Identifier", "Equals", "Colon", "Comma", "Operator", "DoubleQuotes", "Semicolon", "OpenParen", "CloseParen", "OpenParen", "CloseCurly", "Keyword", "TypeKeyword" };
const std::array<std::string, 16> tokenTypes = { "Invalid", "EndOfFile", "Number", "Identifier", "Equals", "Colon", "Comma", "Operator", "DoubleQuotes", "Semicolon", "OpenParen", "CloseParen", "OpenParen", "CloseCurly", "Keyword", "TypeKeyword" };
const std::array<std::string, 9> nodeTypes = { "Program", "VarDeclaration", "AssigmentExpr", "BinaryExpr", "UnaryExpr", "NumericLiteral", "Identifier", "Property", "ObjectLiteral" };
const std::array<std::string, 4> valueTypes = { "Null", "Number", "Boolean", "Object" };

const std::unordered_map<std::string, std::string> color_hash = {
	{ "reset",    "\033[0m"  },
	{ "red",      "\033[31m" },
	{ "green",    "\033[32m" },
	{ "yellow",   "\033[33m" },
	{ "blue",     "\033[34m" },
	{ "gray",     "\033[90m" },
	{ "white",    "\033[37m" },

	{ "bold_red",   "\033[1;31m" },
	{ "bold_green", "\033[1;32m" }
};

namespace Utils {
	const inline std::string token_type_as_string(const TokenType type) {
		return tokenTypes.at(static_cast<std::array<std::string, 15>::size_type>(type));
	}

	const inline std::string node_kind_as_string(const NodeType kind) {
		return nodeTypes.at(static_cast<std::array<std::string, 9>::size_type>(kind));
	}

	const inline std::string value_type_as_string(const ValueType type) {
		return valueTypes.at(static_cast<std::array<std::string, 4>::size_type>(type));
	}


	inline std::string colorize(const std::string& text, const std::string& color)  {
		return color_hash.at(color) + text + color_hash.at("reset");
	}

	std::string get_file_content(std::ifstream& file);
}