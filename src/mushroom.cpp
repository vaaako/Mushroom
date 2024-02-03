#include "headers/mushroom.hpp"
#include "headers/debug.hpp"
#include "headers/environment.hpp"
#include "headers/error.hpp"
#include "headers/interpreter.hpp"
#include "headers/lexer.hpp"
#include "headers/file.hpp"
#include "headers/node.hpp"
#include "headers/parser.hpp"
#include "headers/values.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>

Mushroom::Mushroom(const std::string& filename, const bool debug) : filename(filename), debug(debug) {
}


bool Mushroom::run(const std::string& source) const {
	// Check if source is empty (if in future I want to get some value as default, put "if(tokens.size() >= 1) return;" this on "make_ast")
	if(source.empty() || std::all_of(source.begin(), source.end(), ::isspace))
		return true;

	File* file = new File(this->filename, source, 0, 1, 0);
	Environment* env = new Environment(*file);

	/**
	 * LEXER */
	Lexer* lexer = new Lexer(*file);

	/**
	 * PARSER */
	Parser* parser = new Parser(lexer, *file);
	
	/**
	 * INTERPRETER */
	Interpreter* interpreter = new Interpreter(parser, env, *file);

	// Check error
	const RuntimeVal* value = interpreter->eval_stmt();
	if(interpreter->has_error()) {
		std::cout << interpreter->get_error() << std::endl;

		delete file;
		delete env;
		delete interpreter; // Lexer and parser deleted here

		return false;
	}


	// DEBUG ONLY //
	if(this->debug) {
		// std::cout << Debug::program(*AST_program.value) << std::endl;
		std::cout << Debug::value(*value) << std::endl;
		// std::cout << Debug::env(*env) << std::endl;
	}
	// DEBUG ONLY //

	delete file;

	delete env;
	delete interpreter; // Lexer and parser deleted here
	delete value;


	return true;
}