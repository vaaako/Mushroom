#include "../headers/parsing/parser.hpp"
#include "../headers/helpers/error.hpp"
#include <cstdio>
#include <stdexcept>

/* 
	Orders of Prescidence
	- AssigmentExpr <- Lower prescidence
	- MemberExpr
	- FunctionCall
	- LogicalExpr
	- ComparasionExpr
	- AdditiveExpr
	- MultiplicitaveExpr
	- UnaryExpr
	- PrimaryExpr <- Valuated first
*/


/**
 * TODO -- Explain
 * 
 * When a error occurs it returns a nullptr and set "error" to the error message
 * When the main loop ends, it also returns a nullptr, so always check if parser has a error when returning a nullptr
 * 
 * Token when has a invalid token return a token of type Invalid (this token is not used, is just a placeholder)
 * and node is built normally, because, since invalid token is not used anywhere, at some point
 * the code will causa an error (almost every time on "parse_primary_expr")
 * 
 * */

Parser::Parser(Lexer* lexer, File& file) : lexer(lexer), file(file) {
	this->init_tokens();
}



const Statement* Parser::make_AST() {
	// Parse until the end of file (EOF)
	const Statement* stmt = nullptr;
	while(this->not_eof()) {
		stmt = this->parse_stmt();

		this->advance();
		return stmt;
	}

	// cToken and nToken will have EOF as token
	// I could delete here, but it would be a wall

	delete this->lexer; // Not used anymore
	this->lexer = nullptr;

	return stmt;
}


const Statement* Parser::parse_stmt() {
	if(this->expected(TokenType::Keyword)) {
		return this->parse_var_declaration();
	}

	return this->parse_expr();
}

const Expr* Parser::parse_expr() { 
	return this->parse_assigment_expr();
}



/**
 * PRESCIDENCE ORDER STARTS HERE */

// Lower prescidence //
const Expr* Parser::parse_assigment_expr() {
	// EXPR = EXPR; or x = 45;
	const Expr* left = this->parse_obj_expr();
	
	// Just return the Expr if not an assigment expr
	if(this->next_token() != TokenType::Equals) {
		return left;
	}


	/**
	 * I can't use advance above, because if I advance and it not equals,
	 * the code will try to advance and tokens will be empty
	 * ^ e.g. - left = "123" -> advance() -> is not '=' -> no tokens -> error */	
	this->advance_twice(); // Get '=' and Expr after '='

	// This will allow chaining (x = y = z)
	const Expr* value = this->parse_assigment_expr();

	return new AssigmentExpr(left, value);
}


// LET IDENT;
// (LET | CONST) IDENT: TYPE  = EXPR;
// let x: number = 123;
// let x: number;
const Statement* Parser::parse_var_declaration() {
	const bool isConstant = (this->cToken->value == "const");
	this->advance(); // Advance to get identitifer

	// No identifier
	if(this->not_expected(TokenType::Identifier)) {
		return this->set_error("Expected some Identifier but got: '" + Utils::token_type_as_string(this->cToken->type) + "'");
	}

	const std::string identifier = this->cToken->value;

	// Check ':'
	this->advance();
	if(this->not_expected(TokenType::Colon)) {
		return this->set_error("':' expected after Identifier '" + identifier + "'");
	}

	// Check type
	this->advance();
	if(this->not_expected(TokenType::ValueType)) {
		return this->set_error("Value Type expected after Identifier '" + identifier + "'");
	}

	const ValueType valueType = valueTypeMap.at(this->cToken->value);
	this->advance();

	// LET IDENT: TYPE; <- This case
	if(this->expected(TokenType::Semicolon)) {
		if(isConstant) {
			return this->set_error("Constant declared but no value was provided!");
		}

		return new VarDeclaration(identifier, valueType, nullptr, false);
	}
	

	// (CONST | LET) IDENT = EXPR; <- This case
	if(this->not_expected(TokenType::Equals)) {
		return this->set_error("Expected '=' or ';' in Variable Declaration, but got '" + this->cToken->value + "'");
	}


	// Get var value
	this->advance();
	const Expr* expr = this->parse_expr();


	// Force semicolon in Variable Declaration
	this->advance();
	if(this->not_expected(TokenType::Semicolon)) {
		delete expr; // Avoid Memory leak
		return this->set_error("Variable Declaration must end with semicolon, but got " + this->cToken->value);
	}

	return new VarDeclaration(identifier, valueType, expr, isConstant);
}


const Expr* Parser::parse_obj_expr() {
	if(this->not_expected(TokenType::OpenCurly))
		return this->parse_add_expr(); // Continue

	// To check if while-loop was break previously with an error
	std::string error_message;
	ObjectLiteral* obj = new ObjectLiteral();


	// Pass curly brace
	this->advance();

	while(this->not_eof() && this->not_expected(TokenType::CloseCurly)) {
		if(this->not_expected(TokenType::Identifier)) {
			delete obj; // Avoid Memory Leak
			return this->set_error("Key expected on Object Literal");
		}

		const std::string key = this->cToken->value;
		this->advance();

		// Allow shorthand
		// Check this case -> { key, key2 }
		if(this->expected(TokenType::Comma)) {
			this->advance();
			obj->insert(new Property(key, nullptr));
			continue;

		// Check this case -> { key }
		} else if(this->expected(TokenType::CloseCurly)) {
			obj->insert(new Property(key, nullptr));
			continue;
		}


		// If not the case above, it has to be this case -> { key: val, key2: val, }
		// Expect key':' <- this colon
		if(this->not_expected(TokenType::Colon)) {
			delete obj; // Avoid Memory Leak
			return this->set_error("Missing ':' or ',' following key '" + key +"' in Object Expr");
		}


		// Get value
		this->advance(); // Advance to get value
		const Expr* value = this->parse_expr();

		// add to object
		obj->insert(new Property(key, value));
		this->advance(); // Advance to check for close or comma

		if(this->not_expected(TokenType::CloseCurly)) {
			// If not endend, it needs to be a comma
			if(this->not_expected(TokenType::Comma)) {
				delete obj; // Avoid Memory Leak
				return this->set_error("Expected comma or closing curly");
			}

			// Next indetifier or comma
			this->advance();
		}

		// If was a Close Curly, loop end
	}

	if(this->not_expected(TokenType::CloseCurly)) {
		delete obj; // Avoid Memory Leak
		return this->set_error("Expected '}' but got '" + this->cToken->value + "'");
	}

	return obj;
}





/* (10 + 5) - 5
How it works:
	- Get left  = 10
	- Get op    = +
	- Get right = 5 
	- Loop
	- Get op    = -
	- Get right = 5
	- Break Loop
*/

// Bind op
const Expr* Parser::parse_add_expr() {
	const Expr* left = this->parse_mul_expr(); // Mul has priority

	// Parse operator
	// Check if next token is operator
	while(this->nToken->value == "+" || this->nToken->value == "-") {
		// Advance to grab op
		this->advance();
		const char op = this->cToken->value[0]; // Grab op (char)

		// Advance to get "right" Expr
		this->advance();
		const Expr* right = this->parse_mul_expr();

		// Build BinaryExpr (as Expr, that way it can bind two BinaryExpr
		// 10 + 5 - 3 -> Left: [10 + 5] -> Right: [- 3] -> [(10 + 5) - 3]
		left = new BinaryExpr(left, right, op);
	}

	return left;
}

const Expr* Parser::parse_mul_expr() {
	const Expr* left = this->parse_primary_expr(); // Primary has even more priority

	// Parse operator
	// Check if next token is operator
	while(this->nToken->value == "*" || this->nToken->value == "/" || this->nToken->value == "%") {
		// Advance to grab op
		this->advance();
		const char op = this->cToken->value[0]; // Grab op (char)

		// Advance to get "right" Expr
		this->advance();
		const Expr* right = this->parse_primary_expr();

		// Build BinaryExpr
		left = new BinaryExpr(left, right, op);
	}

	return left;
}

// Get Node type (primary)
const Expr* Parser::parse_primary_expr() {
	switch (this->cToken->type) {
		case TokenType::Identifier:
			return new Identifier(this->cToken->value);

		case TokenType::Number:
			return new NumericLiteral(std::stod(this->cToken->value));


		case TokenType::Operator: {
			// Make unary
			if(this->expected("+") || this->expected("-")) {
				const char op = this->cToken->value[0];
				this->advance(); // Advance to get number
				const Expr* num = this->parse_primary_expr();

				return new UnaryExpr(num, op);
			}
		}

		case TokenType::OpenParen: {
			this->advance(); // Advance to get the value inside parentheses
			const Expr* value = this->parse_expr();
			
			// Advance to check if next token is what expected
			this->advance();
			if(this->not_expected(TokenType::CloseParen)) {
				delete value; // Prevent memory leak on error (since this value will not be returned)
				return this->set_error("Expected ')' but got '" + this->cToken->value + "'");
			}

			return value;
		}

		default:
			return this->set_error(
				"Expected some <Keyword>, <Identifier>, <Symbol>, <Operator> or '(', but got invalid token '"
				+ this->cToken->value + "'");
	}
}