#pragma once

#include "error.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "response.hpp"
#include "token.hpp"
#include <optional>
#include <stdexcept>

// TODO -- Make a vector of tokens, if have an error, delete all, maybe use program for this
// TODO -- or come back with normal parser

// TODO -- If, tokenize had error, set parser.error = true, then check at the end of make_AST, if had, delete all and throw, can do the same with interpreter

class Parser {
	public:
		Parser(Lexer* lexer, File& file);
		~Parser() {
			if(this->cToken != nullptr)
				delete this->cToken;
	
			if(this->nToken != nullptr)
				delete this->nToken;
		}

		// const Response<const Program*> make_AST(File& file);
		const Statement* make_AST();

		
		// Need to run before starting parser
		inline void init_tokens() {
			// Init Tokens
			this->cToken = this->lexer->tokenize(); 
			this->nToken = this->lexer->tokenize();

			// if(this->cToken == nullptr || this->nToken == nullptr) {
				// Since I don't know wich cause the error, delete both
				// delete cToken;
				// delete nToken;

				// throw std::runtime_error("INIT Invalid token value");
			// }
		}

		inline bool has_error() {
			return !this->error.empty();
		}

		const inline std::string& get_error() {
			return this->error;
		}
	private:
		Lexer* lexer;
		File& file;

		std::string error;
		const Token* cToken; // Current token
		const Token* nToken; // Next token


		const std::unordered_map<std::string, ValueType> valueTypeMap = {
			{ "nil",   ValueType::Null }, // TODO -- Test only
			{ "number", ValueType::Number }, // TODO -- Make int and float support later
			{ "bool",   ValueType::Boolean },
			{ "object",   ValueType::Object }
		};

		const Statement* parse_stmt();
		const Expr* parse_expr();

		const Expr* parse_assigment_expr();
		const Expr* parse_obj_expr();
		const Statement* parse_var_declaration();

		const Expr* parse_add_expr(); // Bind op
		const Expr* parse_mul_expr(); // Bind op
		const Expr* parse_primary_expr(); // <-- Higher prescidence




		//          //
		// Checkers //
		//          //
		inline bool not_eof() const {
			return this->cToken->type != TokenType::EndOfFile;
		}

		inline TokenType next_token() const {
			return this->nToken->type;
		}

		// true | false
		inline bool expected(const TokenType expec) const {
			return this->cToken->type == expec;
		}

		inline bool expected(const std::string& expec) const {
			return this->cToken->value == expec;
		}

		inline bool not_expected(const TokenType expec) const {
			return this->cToken->type != expec;
		}



		//         //
		// Actions //
		//         //
		inline std::nullptr_t set_error(const std::string& message) {
			this->error = Error::make_error(Error::Type::InvalidSyntaxError,
				message, this->file);

			return nullptr;
		}

		inline void advance() {
			delete this->cToken; // Delete "previous" token

			this->cToken = this->nToken; // Get "current" token
			this->nToken = this->lexer->tokenize();

			// If get an invalid token
			// if(this->nToken == nullptr) {
				// if(this->cToken != nullptr)
				// delete this->cToken;
				// throw std::runtime_error("ADVANCE Invalid token value");
			
				// response.had_error("Invalid Token value");
			// }
		}

		inline void advance_twice() {
			this->advance();
			this->advance();
		}
};

/*
NUM OP NUM
cToken = NUM
nToken = OP
-> delete cToken NUM

cToken = nullptr
cToken = OP
nToken = NUM
-> delete cToken OP

cToken = NUM
nToken = EOF
-> delete cToken NUM

cToken = EOF
nToken = EOF
*/
