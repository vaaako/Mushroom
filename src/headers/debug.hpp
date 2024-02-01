#pragma once

#include "environment.hpp"
#include "node.hpp"
#include "token.hpp"
#include "values.hpp"

/**
 * DEBUG AREA */

// @WARNING THE CODE BELOW CAN HURT YOUR EYES AND BLIND YOU
// I DIDN'T EVEN CARA TO MAKE THE CODE GOOD, IT IS A HORRIBLE CODE BECAUSE I AM TOO
// LAZY TO EVEN CARE TO MAKE THIS PART OF THE PROGRAM GOOD, YOU ARE WARNED

namespace Debug {
	const std::string token(const Token&);
	const std::string program(const Program& program);
	const std::string value(const RuntimeVal& value, Environment* env=nullptr);
	const std::string env(Environment& env);

	const std::string get_node_value(const Statement& stmt);
	const std::string runtimeValue_to_string_colorized(const RuntimeVal& value, Environment* env=nullptr);
	

	// const std::string debug_all(std::queue<const Token*> tokens);
}
