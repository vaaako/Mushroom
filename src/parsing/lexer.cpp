#include "../headers/parsing/lexer.hpp"

Lexer::Lexer(File& file) : file(file) {
	this->source = file.source;

	// Pop char from text if not empty
	this->ch = this->pop_source();
}

const Token* Lexer::tokenize() {
	// Build token until end of the file (or not)
	while(this->ch != '\0') {
		// Can skip
		if(this->isskippable()) {
			this->advance();
			continue;

		// } else if(this->ch == '/' && this->next_ch() == '/') {
		// 	this->advance_comment();
		// 	continue;
		// }

		// Comment
		} else if(this->ch == '/') { // Long comment?
			// Normal comment
			if(this->next_ch() == '/') {
				this->advance_comment();
				continue;
			}

			// Long comment
			// } else if(this->next_ch() == '*') {
			// 	this->long_comment();
			// 	continue;
			// }
		}


		// Build number
		if(std::isdigit(this->ch)) {
			return this->build_num();

		// Build Identifier
		} else if(std::isalpha(this->ch)) {
			return this->build_identifier();
		}

		// Build one character token
		short ttype = this->token_type_from_char();

		// Token not found
		if(ttype == -1) {
			return new Token(TokenType::Invalid, std::string(1, this->ch));
		}

		const Token* token = new Token(static_cast<TokenType>(ttype), std::string(1, this->ch));
		this->advance();

		return token;
	}

	// Insert EOF
	return new Token(TokenType::EndOfFile, "EOF");
}


const Token* Lexer::build_num() {
	std::string num_str;
	bool has_dot = false;

	// While not the end
	while(this->ch != '\0') {
		// Is digit, increment
		if(std::isdigit(this->ch)) {
			num_str += this->ch;

		// Build dot
		} else if(this->ch == '.') {
			if(has_dot) break; // Can't have more than one dot
			has_dot = true;
			num_str += '.';

		// Ignore if underline
		} else if(this->ch == '_') {
			this->advance();
			continue;

		// Not a number anymore, break
		} else { 
			break;
		}

		this->advance();
	}

	return new Token(TokenType::Number, num_str);
}


const Token* Lexer::build_identifier() {
	std::string id_str = "";

	while(this->ch != '\0') {
		// Check is a valid identifier character
		if(!this->isidentifier())
			break;

		id_str += this->ch;

		this->advance();
	}

	// Check if identifier is a keyword
	TokenType tok_type = this->keyword_or_identifier(id_str);

	return new Token(tok_type, id_str);
}