#include "../headers/environment.hpp"
#include "../headers/utils.hpp"
#include <stdexcept>

Environment::Environment(File& file, Environment* parent) : file(file), parent(parent) {
	this->setup_globals();
}


// TODO TODO -- Make each one return nullptr if have an error

// #include <iostream>
// #include "../headers/debug.hpp"

const RuntimeVal* Environment::declare_var(const std::string& symbol, const RuntimeVal* value, const bool isConst) {
	if(this->var_exists(symbol)) {
		delete value; // Avoid Memory Leak
		throw std::runtime_error("Cannot declare variable '" + symbol + "' because it's alredy declared");
	}

	// std::cout << "Set symbol " + symbol + " with value of ";
	// std::cout << Debug::runtimeValue_to_string_colorized(*value) << std::endl;


	// Clone value to symbol table
	this->symbolTable.emplace(symbol, value->clone());

	// std::cout << "Added " + symbol + " with value of ";
	// std::cout << Debug::runtimeValue_to_string_colorized(*this->symbolTable.at(symbol)) << std::endl;

	if(isConst) {
		this->constMap.emplace(symbol);
	}

	return value;
}

const RuntimeVal* Environment::assign_var(const std::string& symbol, const RuntimeVal* value) {
	// Get the current Environment
	Environment* env = this->resolve(symbol);
	if(env == nullptr) {
		delete value; // Avoid Memory Leak
		throw std::runtime_error("Cannot find Identifier '" + symbol + "'");
	}

	if(!env->var_exists(symbol)) {
		delete value; // Avoid Memory Leak
		throw std::logic_error("<environment.cpp> Cannot assign to '" + symbol + "' because doens't exist");
	}


	if(env->constMap.find(symbol) != this->constMap.end()) {
		throw std::runtime_error("Cannot assign a new value to a Constant '" + symbol + "'");
	}

	const RuntimeVal* old_value = env->symbolTable.at(symbol);

	// Check if new value is the same type of old value
	if(value->type != old_value->type) {
		delete value; // Avoid Memory Leak
		throw std::runtime_error("The variable '" + symbol
				+ "' is of type " + Utils::value_type_as_string(old_value->type)
				+ " and you are trying to assigne a value of type " + Utils::value_type_as_string(value->type)
		);
	}

	// std::cout << "Replaced symbol " + symbol + " with old value of " + Debug::runtimeValue_to_string_colorized(*old_value)
		// + " and new value of " + Debug::runtimeValue_to_string_colorized(*value)  << std::endl;


	// Change value
	delete old_value;

	env->symbolTable.erase(symbol); // Erase old from map
	env->symbolTable.emplace(symbol, value->clone()); // Add new one (aka replace)

	return value;
}


const RuntimeVal* Environment::look_up(const std::string& symbol) {
	const Environment* env = this->resolve(symbol);
	if(env == nullptr) {
		throw std::runtime_error("Cannot find Identifier '" + symbol + "'");
	}

	if(!env->var_exists(symbol))
		throw std::logic_error("<environment.cpp> Var " + symbol + " doens't exist!");

	// std::cout << "Query: " + symbol + " Found: " + Debug::runtimeValue_to_string_colorized(*env->symbolTable.at(symbol)) << std::endl;

	return env->symbolTable.at(symbol);
}





Environment* Environment::resolve(const std::string& symbol) {
	if(this->var_exists(symbol))
		return this;
	
	if(this->parent == nullptr) {
		return nullptr;
	}
	
	return this->parent->resolve(symbol);
}

