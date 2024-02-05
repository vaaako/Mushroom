#include "../headers/types/token.hpp"

Token::Token(const TokenType type, const std::string& value)
	: type(type), value(value) {}

