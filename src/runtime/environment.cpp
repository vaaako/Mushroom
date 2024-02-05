#include "../headers/runtime/environment.hpp"
#include "../headers/helpers/utils.hpp"
#include <stdexcept>

Environment::Environment(File& file, Environment* parent) : file(file), parent(parent) {
	this->setup_globals();
}


const RuntimeVal* Environment::declare_var(const std::string& symbol, const RuntimeVal* value, const bool isConst) {
	if(this->var_exists(symbol))
		return this->set_error("Cannot declare variable '" + symbol + "' because it's alredy declared");

	// Clone value to symbol table
	this->symbolTable.emplace(symbol, value->clone());

	if(isConst)
		this->constMap.emplace(symbol);

	return value;
}

const RuntimeVal* Environment::assign_var(const std::string& symbol, const RuntimeVal* value) {
	// Get the current Environment
	Environment* env = this->resolve(symbol);
	if(env == nullptr)
		return this->set_error("AAA Cannot find Identifier '" + symbol + "'");

	// Never happening
	if(!env->var_exists(symbol))
		throw std::logic_error("<environment.cpp> Cannot assign to '" + symbol + "' because doens't exist");

	if(env->constMap.find(symbol) != this->constMap.end())
		return this->set_error("Cannot assign a new value to a Constant '" + symbol + "'");


	// Check if new value is the same type of old value
	const RuntimeVal* old_value = env->symbolTable.at(symbol);
	if(value->type != old_value->type) {
		return this->set_error("The variable '" + symbol
				+ "' is of type <" + Utils::value_type_as_string(old_value->type)
				+ "> and you are trying to assigne a value of type <" + Utils::value_type_as_string(value->type) + ">"
		);
	}

	// Replacing
	delete old_value; // Delete old
	env->symbolTable.erase(symbol); // Erase old from map
	env->symbolTable.emplace(symbol, value->clone()); // Add new one (aka replace)

	return value;
}


const RuntimeVal* Environment::look_up(const std::string& symbol) {
	const Environment* env = this->resolve(symbol);
	if(env == nullptr)
		return this->set_error("Cannot find Identifier '" + symbol + "'");

	if(!env->var_exists(symbol))
		throw std::logic_error("<environment.cpp> Var " + symbol + " doens't exist!");

	return env->symbolTable.at(symbol);
}





Environment* Environment::resolve(const std::string& symbol) {
	if(this->var_exists(symbol))
		return this;
	
	if(this->parent == nullptr)
		return nullptr;
	
	return this->parent->resolve(symbol);
}

