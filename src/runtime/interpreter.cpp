#include "../headers/interpreter.hpp"
#include "../headers/error.hpp"
#include "../headers/utils.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>

// TODO -- File later will be part of nodes (so will be possible to handle errors in other files)
Interpreter::Interpreter(Parser* parser, Environment* env, File& file) : parser(parser), env(env), file(file) {}


// Evaluate each Statement
// const RuntimeVal* Interpreter::eval_program(const Program* program) const {
// 	const RuntimeVal* lastEval = new NullVal(); // Null is default value

// 	// Eval all Statements
// 	for(const Statement* stmt : program->body) {
// 		delete lastEval; // Delete last to not get memory leak on setting a new one
// 		lastEval = this->evaluate(stmt);
// 	}

// 	return lastEval;
// }

// TODO -- Something simlar to parser
const Response<const RuntimeVal*> Interpreter::eval_stmt() const {
	const RuntimeVal* lastEval = new NullVal(); // Null is default value
	Response<const RuntimeVal*> response = Response<const RuntimeVal*>();

	// Eval all Statements
	const Statement* stmt = nullptr;
	while((stmt = this->parser->make_AST()) != nullptr) {
		delete lastEval; // Delete last to not get memory leak on setting a new one
		// lastEval = nullptr; // In case of error, to check if was deleted alredy
		lastEval = this->evaluate(stmt);

		delete stmt; // Avoid Memory Leak
	}

	// If loop ended, is over or had an error
	if(this->parser->has_error()) {
		if(lastEval != nullptr)
			delete lastEval;

		response.had_error(this->parser->get_error());
		return response;
	}

	response.set_value(lastEval);
	return response;

}




const RuntimeVal* Interpreter::eval_vardec(const VarDeclaration* vardec) const {
	// Get var value
	// WARNING -- When this value changes, it will change on ENV, because it is pointing to "value" (if not store as a copy)
	const RuntimeVal* value = (vardec->value) ? this->evaluate(vardec->value) : new NullVal();

	// Var type can be anything and value can be null, but not the opposite
	if(value->type != ValueType::Null && value->type != vardec->valueType) {
		const ValueType valuetype = value->type; // Store before deleting
		delete value; // Avoid Memory Leaks

		throw std::runtime_error("The variable '" + vardec->identifier
				+ "' takes Value Type of " + Utils::value_type_as_string(vardec->valueType)
				+ " but the value is of type " + Utils::value_type_as_string(valuetype)
		);
	}

	// If has value, evaluate and get it to pass to Envrionment, else, just pass a null
	// And remove const	
	return this->env->declare_var(vardec->identifier, value, vardec->constant);
}

const RuntimeVal* Interpreter::eval_assign(const AssigmentExpr* node) const {
	if(node->assigne->kind != NodeType::Identifier) {
		throw std::runtime_error("You can't assigne to "
			+ Utils::node_kind_as_string(node->assigne->kind)
			+ ", only to <Identifier> kind"
		);
	}


	return env->assign_var(
		static_cast<const Identifier*>(node->assigne)->symbol,
		this->evaluate(node->value)
	);
}

// Get left-hand side and right-hand side and evaluate
const RuntimeVal* Interpreter::eval_binop(const BinaryExpr* binop) const {
	const RuntimeVal* left  = this->evaluate(binop->left);
	const RuntimeVal* right = this->evaluate(binop->right);

	// If number, calc
	if(left->type == ValueType::Number && right->type == ValueType::Number)
		return this->eval_num_binop(static_cast<const NumberVal*>(left), static_cast<const NumberVal*>(right), binop->operatorChar);


	// Can delete because will not be used
	delete left;
	delete right;

	// If operation is invalid (e.g. 1 + null)
	return new NullVal();
}

const RuntimeVal* Interpreter::eval_unary(const UnaryExpr* unary) const {
	const RuntimeVal* expr = this->evaluate(unary->expr);

	// The default value of "expr" will be positive, so check if Unary has operator "-" and invert result (get the negative result of "expr")
	if(unary->operatorChar == '-') {
		// Get the negative value of "expr"
		// I don't need to delete "new NumberVal(-1)" because is deleted inside "eval_num_binop"
		return this->eval_num_binop(new NumberVal(-1), static_cast<const NumberVal*>(expr), '*');
	}

	// If "+X" just return the positive result
	return expr;
}

const RuntimeVal* Interpreter::eval_num_binop(const NumberVal* left, const NumberVal* right, const char& op) const {
	double result = 0;

	switch (op) {
		case '+':
			result = left->value + right->value;
			break;

		case '-':
			result = left->value - right->value;
			break;

		case '*':
			result = left->value * right->value;
			break;

		case '/':
			// TODO -- division by zero check
			result = left->value / right->value;
			break;

		case '%':
			result = fmod(left->value, right->value);
			break;

		default:
			throw std::logic_error("<interpreter.cpp> Undefined operator: " + std::string(1, op));
	}

	// Left and right was just used for the calc
	delete left;
	delete right;

	return new NumberVal(result);
}



const RuntimeVal* Interpreter::eval_ident(const Identifier* ident) const {	
	// Get a clone of identifier in Environment, because it will be used as value and this avoid memory leak
	// And variables in Environment should stay there
	return this->env->look_up(ident->symbol)->clone();
}


const RuntimeVal* Interpreter::eval_object(const ObjectLiteral* obj) const {
	// ObjectVal* object = new ObjectVal(); // Is the value of VarDeclaration, so it will be deleted when the var get deleted
	std::unordered_map<std::string, const std::shared_ptr<const RuntimeVal>> properties;


	// TODO -- memory leak if don't find indetifier in look_up
			// object will not be deleted

	// For all values inside obj
	for(const Property* prop : obj->properties) {
		const RuntimeVal* value = (prop->value == nullptr) // -> { foo } -> get foo value from environment
			? this->env->look_up(prop->key)->clone() // This is deleted when ObjectLiteral is deleted, so make a clone to avoid double delete
			// ? nullptr // This is deleted when ObjectLiteral is deleted and I can get it from environment instead of storing on ObjectVal
			: this->evaluate(prop->value);

		properties.emplace(prop->key, std::shared_ptr<const RuntimeVal>(value));
	}

	return new ObjectVal(properties);
}


// Eval each AST nodes
const RuntimeVal* Interpreter::evaluate(const Statement* ast_node) const {
	switch (ast_node->kind) {
		// Statements
		// case NodeType::Program:
		// 	return this->eval_program(static_cast<const Program*>(ast_node));
		
		case NodeType::VarDeclaration:
			return this->eval_vardec(static_cast<const VarDeclaration*>(ast_node));

		// Expressions
		case NodeType::AssigmentExpr:
			return this->eval_assign(static_cast<const AssigmentExpr*>(ast_node));
		
		case NodeType::BinaryExpr:
			return this->eval_binop(static_cast<const BinaryExpr*>(ast_node));
		
		case NodeType::UnaryExpr:
			return this->eval_unary(static_cast<const UnaryExpr*>(ast_node));

		// Literals
		case NodeType::NumericLiteral:
			return new NumberVal(static_cast<const NumericLiteral*>(ast_node)->value);
		
		case NodeType::Identifier:
			return this->eval_ident(static_cast<const Identifier*>(ast_node));

		case NodeType::ObjectLiteral:
			return this->eval_object(static_cast<const ObjectLiteral*>(ast_node));


		default:
			throw std::logic_error("<interpreter.cpp> Node of type " + std::to_string(static_cast<int>(ast_node->kind)) + " not implemented yet");
	}
}
