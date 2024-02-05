#pragma once

#include <string>

// WARNING -- IF ADD A NEW, ALSO ADD TO "tokenTypes"
enum class TokenType {
	// Invisible Tokens
	Invalid,
	EndOfFile,

	// Multicharacter
	Number,
	Identifier,

	// Symbols
	Equals,
	Colon,
	Comma,
	Operator,
	DoubleQuotes,
	Semicolon,

	OpenParen,
	CloseParen,
	OpenCurly,
	CloseCurly,

	// Keywords
	Keyword,
	ValueType,
	// Let,
	// Const
};

struct Token {
	const TokenType type;
	const std::string value;

	Token(const TokenType type, const std::string& value);
};