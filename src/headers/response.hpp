#pragma once

#include <string>
#include <variant>

template <typename T = std::monostate> // std::monostate -> Can be void
struct Response {
	T value;
	bool has_error = false;
	std::string errorMessage;

	Response() {}
	~Response() = default;

	inline void set_value(const T value) {
		this->value = value;
	}

	inline void had_error(const std::string& errorMessage) {
		this->errorMessage = errorMessage;
		this->has_error = true;
	}
};
