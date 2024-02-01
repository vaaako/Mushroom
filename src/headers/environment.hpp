#pragma once

#include "file.hpp"
#include "values.hpp"
#include <unordered_map>
#include <unordered_set>

class Environment {
	public:
		Environment(File& file, Environment* parent=nullptr);
		~Environment() {
			for(auto it : this->symbolTable) {
				delete it.second;
			}
			this->symbolTable.clear();
		}
		
		const RuntimeVal* declare_var(const std::string& symbol, const RuntimeVal*, const bool isConst = true);
		const RuntimeVal* assign_var(const std::string& symbol, const RuntimeVal* value); // Change var value

		const RuntimeVal* look_up(const std::string& symbol);

		const inline std::unordered_map<std::string, const RuntimeVal*> get_symbols() {
			return symbolTable;
		}
	private:
		File& file;
		Environment* parent = nullptr;

		std::unordered_map<std::string, const RuntimeVal*> symbolTable;
		std::unordered_set<std::string> constMap; // All constants

		Environment* resolve(const std::string& symbol);


		// PROVISORY //
		// Will need to change when add support to import other files
		inline void setup_globals() {
			// Global vars
			this->raw_var("x",     new NumberVal(666)); // DEV ONLY //
			this->raw_var("null",  new NullVal());
			this->raw_var("true",  new BooleanVal(true));
			this->raw_var("false", new BooleanVal(false));
		}

		inline void raw_var(const std::string& symbol, const RuntimeVal* value) {
			this->symbolTable.emplace(symbol, value);
		}
		// PROVISORY //


		inline bool var_exists(const std::string& symbol) const {
			return this->symbolTable.find(symbol) != this->symbolTable.end();
		}
};