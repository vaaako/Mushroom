#pragma once

#include "environment.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "values.hpp"

class Interpreter {
	public:
		Interpreter(Parser* parser, Environment* env, File& file);
		
		const RuntimeVal* evaluate(const Statement* ast_node) const;
		// const RuntimeVal* eval_stmt() const;

		const Response<const RuntimeVal*> eval_stmt() const;
	private:
		Parser* parser;
		Environment* env;
		File& file;

		const RuntimeVal* eval_program(const Program* program) const;
		const RuntimeVal* eval_vardec(const VarDeclaration* vardec) const;
		const RuntimeVal* eval_assign(const AssigmentExpr* node) const;
		const RuntimeVal* eval_binop(const BinaryExpr* binop) const;
		const RuntimeVal* eval_unary(const UnaryExpr* unary) const;
		const RuntimeVal* eval_num_binop(const NumberVal* left, const NumberVal* right, const char& op) const;
		const RuntimeVal* eval_ident(const Identifier* identifier) const;
		const RuntimeVal* eval_object(const ObjectLiteral* obj) const;


};