#pragma once

#include "environment.hpp"
#include "../helpers/error.hpp"
#include "../types/node.hpp"
#include "../parsing/parser.hpp"
#include "../types/values.hpp"

class Interpreter {
	public:
		Interpreter(Parser* parser, Environment* env, File& file);
		~Interpreter() {
			delete parser;
		}
		
		const RuntimeVal* evaluate(const Statement* ast_node);
		// const RuntimeVal* eval_stmt() const;

		const RuntimeVal* eval_stmt();

		inline bool has_error() {
			return !this->error.empty();
		}

		const inline std::string& get_error() {
			return this->error;
		}
	private:
		Parser* parser;
		Environment* env;
		File& file;
		std::string error;

		inline std::nullptr_t set_error(const std::string& message, const bool build = true) {
			// This was probably called by a parser error
			if(!build) {
				this->error = message;
				return nullptr;
			}

			this->error = Error::make_error(Error::Type::RuntimeError,
				message, this->file);
			return nullptr;
		}

		const RuntimeVal* eval_program(const Program* program) const;
		const RuntimeVal* eval_vardec(const VarDeclaration* vardec);
		const RuntimeVal* eval_assign(const AssigmentExpr* node);
		const RuntimeVal* eval_binop(const BinaryExpr* binop);
		const RuntimeVal* eval_unary(const UnaryExpr* unary);
		const RuntimeVal* eval_num_binop(const NumberVal* left, const NumberVal* right, const char& op) const;
		const RuntimeVal* eval_ident(const Identifier* identifier);
		const RuntimeVal* eval_object(const ObjectLiteral* obj);
};