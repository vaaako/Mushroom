#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../helpers/file.hpp"
#include "../types/token.hpp"

class Lexer {
	public:
		Lexer(File& file);

		const Token* tokenize();
	private:
		char ch; // Current character
		std::string source; // Current source
		File& file;

		const std::unordered_set<std::string> keywordSet = {
			"let", "const"
		};

		const std::unordered_set<std::string> valueTypeSet = {
			"nil", "number", "bool", "obj"
		};

		const std::unordered_set<char> operatorSet = { '+', '-', '/', '*', '^', '%', '>', '<' };
		const std::unordered_map<char, TokenType> symbolMap = {
			{ '=', TokenType::Equals },
			{ ':', TokenType::Colon },
			{ ',', TokenType::Comma },
			{ '"', TokenType::DoubleQuotes },
			{ ';', TokenType::Semicolon },
			{ '(', TokenType::OpenParen },
			{ ')', TokenType::CloseParen },
			{ '{', TokenType::OpenCurly },
			{ '}', TokenType::CloseCurly }
		};


		const Token* build_num();
		const Token* build_identifier();



		//          //
		// Checkers //
		//          //
		inline bool isskippable() const {
			return std::isspace(this->ch) || this->ch == ' ' || this->ch == '\t' || this->ch == '\n' || this->ch == '\r';
		}

		inline bool isidentifier() const {
			return std::isalnum(this->ch) || this->ch == '_';
		}


		inline bool two_char(const char& ch, const char& nc) {
			return this->ch == ch && this->next_ch() == nc;
		}

		inline TokenType keyword_or_identifier(const std::string& symbol) const {
			if(this->keywordSet.count(symbol))
				return TokenType::Keyword;
			else if(this->valueTypeSet.count(symbol))
				return TokenType::ValueType;
			else
				return TokenType::Identifier;
		}


		//         //
		// Getters //
		//         //
		inline short token_type_from_char() const {
			// Check if ch is an operator
			if(operatorSet.count(ch) > 0)
				return (short)TokenType::Operator;

			// Check if is symbol with own enum
			auto it = symbolMap.find(ch);
			return (it == symbolMap.end()) ? -1 : static_cast<short>(it->second);
		}

		inline char next_ch() {
			return this->source.at(0);
		}



		//         //
		// Actions //
		//         //
		inline void advance_comment() {
			// Until end of line
			while(this->ch != '\n' && this->ch != '\0') {
				this->advance();
			}
		}

		// inline void long_comment() {
		// 	// Until close comment
		// 	while(!this->two_char('*', '/')) {
		// 		this->advance();
		// 	}
		// }

		// Go to next character
		inline void advance() {
			this->file.advance(this->ch);
			this->ch = (!this->source.empty()) ? this->pop_source() : '\0'; // If reach last character change to 'end character' to break loop
		}

		inline char pop_source() {
			const char c = this->source.at(0);
			this->source.erase(this->source.begin());
			return c;
		}
};
