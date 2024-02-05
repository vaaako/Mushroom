#pragma once

#include "../helpers/error.hpp"
#include "../helpers/file.hpp"
#include "../types/values.hpp"
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

		inline bool has_error() {
			return !this->error.empty() || !(this->parent && this->parent->has_error());
		}

		const inline std::string get_error() {
			return (this->has_error()) ? this->error : this->parent->get_error();
		}
	private:
		File& file;
		Environment* parent = nullptr;
		std::string error;

		std::unordered_map<std::string, const RuntimeVal*> symbolTable;
		std::unordered_set<std::string> constMap; // All constants

		Environment* resolve(const std::string& symbol);


		// PROVISORY //
		// Will need to change when add support to import other files
		inline void setup_globals() {
			// Global vars
			this->raw_var("null",  new NullVal());
			this->raw_var("true",  new BooleanVal(true));
			this->raw_var("false", new BooleanVal(false));
		}

		inline void raw_var(const std::string& symbol, const RuntimeVal* value) {
			this->symbolTable.emplace(symbol, value);
			this->constMap.emplace(symbol);
		}
		// PROVISORY //

		inline bool var_exists(const std::string& symbol) const {
			return this->symbolTable.find(symbol) != this->symbolTable.end();
		}



		inline std::nullptr_t set_error(const std::string& message) {
			this->error = Error::make_error(Error::Type::RuntimeError,
				message, this->file);

			return nullptr;
		}
};