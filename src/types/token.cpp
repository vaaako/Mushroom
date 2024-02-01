#include "../headers/token.hpp"

Token::Token(const TokenType type, const std::string& value)
	: type(type), value(value) {}

// Token::Token(const TokenType type, const std::string& value,
// 			 const Position& pos_start)
// 	: type(type), value(value), pos_start(pos_start), pos_end(pos_start) {}


// Token::Token(const TokenType type, const std::string& value,
// 			 const Position& pos_start, Position& pos_end)
// 	: type(type), value(value), pos_start(pos_start), pos_end(pos_end) {

// 	this->pos_end.advance();
// }

