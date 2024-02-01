#include "../headers/debug.hpp"

#include "../headers/utils.hpp"
#include <stdexcept>

// const Debug::debug_all(std::queue<const Token *> tokens) {
// 	std::string result;

// 	// TOKENS //
// 	result += "Tokens Length: " + std::to_string(tokens.size()) + "\nTYPE\tVALUE\n";
	

// 	while(!tokens.empty()) {
// 		const Token* token = tokens.front();

// 		result += Utils::colorize(Utils::token_type_as_string(token->type), "bold_green")
// 			   + "    \t |   "
// 			   + Utils::colorize(token->value, "yellow")
// 			   + "    \t | " + std::to_string(sizeof(*token)) + " bytes"
// 			   + "\n";

// 		tokens.pop();
// 	}
// 	// TOKENS //

// 	return result;
// }

const std::string Debug::token(const Token& token) {
	return Utils::colorize(Utils::token_type_as_string(token.type), "bold_green")
		+ "    \t |   "
		+ Utils::colorize(token.value, "yellow")
		+ "    \t | " + std::to_string(sizeof(token)) + " bytes";
}

const std::string Debug::program(const Program& program) {
	std::string result = "Nodes Length: " + std::to_string(program.body.size()) + "\n";
	
	for(const Statement* stmt : program.body) {
		// Setup colors
		result += Utils::colorize("Kind: ", "bold_green")
			   +  Utils::colorize(Utils::node_kind_as_string(stmt->kind), "yellow")
			   +  " | "
			   +  Utils::colorize("Value: ", "bold_green")
			   +  Utils::colorize(get_node_value(*stmt), "yellow")
			   +  "\n";
	}

	return result;
}

const std::string Debug::value(const RuntimeVal& value, Environment* env) {
	return "EVAL RESULT\n" + Debug::runtimeValue_to_string_colorized(value, env) + "\n";
}

const std::string Debug::env(Environment& env) {
	std::string result = "ENVRIONMENT RESULT \nLENGTH: " + std::to_string(env.get_symbols().size()) + "\n";

	for(auto& entry : env.get_symbols()) {
		result += Utils::colorize("Kind: ", "bold_green")
		   +  Utils::colorize(entry.first, "yellow")
		   +  " \t | "
		   +  Utils::colorize("Value: ", "bold_green")
		   + runtimeValue_to_string_colorized(*entry.second)
		   // + runtimeValue_to_string_colorized(entry.second)
		   // + runtimeValue_to_string_colorized(*entry.second.get())
		   + "\n";
	}

	return result;
}


const std::string Debug::get_node_value(const Statement& stmt) {
	// Get value of node type
	// Attempt to cast to BinaryExpr
	switch (stmt.kind) {
		case NodeType::VarDeclaration: {
			const VarDeclaration& vardec = static_cast<const VarDeclaration&>(stmt);
			return ((vardec.constant) ? "const " : "let ")
				+ vardec.identifier + ": " + Utils::value_type_as_string(vardec.valueType)
				+ ((vardec.value != nullptr) ? " = " + get_node_value(*vardec.value) : ";");
		}

		case NodeType::AssigmentExpr: {
			const AssigmentExpr& assig = static_cast<const AssigmentExpr&>(stmt);
			return get_node_value(*assig.assigne)
				+ " = " + get_node_value(*assig.value);
		}

		case NodeType::BinaryExpr: {
			const BinaryExpr& binaryExpr = static_cast<const BinaryExpr&>(stmt);
			return get_node_value(*binaryExpr.left)
				+  " " + std::string(1, binaryExpr.operatorChar)
				+  " " + get_node_value(*binaryExpr.right);
		}

		case NodeType::UnaryExpr: {
			const UnaryExpr& unaryExpr = static_cast<const UnaryExpr&>(stmt);
			return std::string(1, unaryExpr.operatorChar)
				+ get_node_value(*unaryExpr.expr);
		}

		case NodeType::NumericLiteral:
			return std::to_string(static_cast<const NumericLiteral&>(stmt).value);

		case NodeType::Identifier:
			return static_cast<const Identifier&>(stmt).symbol;

		case NodeType::Property: {
			const Property& prop = static_cast<const Property&>(stmt);
			return (prop.value == nullptr)
					? prop.key
					: prop.key + ": " + get_node_value(*prop.value);
		}

		case NodeType::ObjectLiteral: {
			// Make a copy to pop
			ObjectLiteral obj = static_cast<const ObjectLiteral&>(stmt);

			std::string result = "{ ";

			for(const Property* prop : obj.properties) {
				result += get_node_value(*prop) + " ";
			}
			result += "} ";

			return result;
		}

		default:
			throw std::logic_error("<utils.hpp> Unknown kind: " + std::to_string(static_cast<short>(stmt.kind)) + " in program.body");
	}

}

const std::string Debug::runtimeValue_to_string_colorized(const RuntimeVal& value, Environment* env) {
	std::string result;

	switch (value.type) {
		case ValueType::Null:
			result += Utils::colorize("null", "yellow");
			break;

		case ValueType::Number:
			result += Utils::colorize(std::to_string(static_cast<const NumberVal&>(value).value), "yellow");
			break;

		case ValueType::Boolean:
			result += Utils::colorize(std::to_string(static_cast<const BooleanVal&>(value).value), "yellow");
			break;

		case ValueType::Object: {
			const ObjectVal& obj = static_cast<const ObjectVal&>(value);

			result += Utils::colorize("{", "green");
			for(const auto& pair : obj.properties) {
				// result += " " + Utils::colorize(pair.first + ": ", "yellow");
				// if(pair.second == nullptr) {
				// 	result += Utils::colorize(runtimeValue_to_string_colorized(*env->look_up(pair.first)), "yellow");
				// 	continue;
				// }
				// result += Utils::colorize(runtimeValue_to_string_colorized(*pair.second.get()), "yellow");

				result += " " + Utils::colorize(pair.first + ": " + runtimeValue_to_string_colorized(*pair.second), "yellow") + ",";
			}
			result += Utils::colorize(" } ", "green");

			break;
		}

		default:
			throw std::logic_error("<utils.hpp> Unknown value of type: " + std::to_string(static_cast<short>(value.type)));
	}

	return result;
}