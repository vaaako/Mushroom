#pragma once

#include "file.hpp"
#include "utils.hpp"
#include <array>
#include <string>


const std::array<std::string, 3> errorTypes = {
	"Illegal Character",
	"Invalid Syntax",
	"Runtime Error"
};


namespace Error {
	enum class Type {
		IllegalCharError,
		InvalidSyntaxError,
		RuntimeError
	};


	// Should be private, just don't use it alone
	const inline std::string highlightError(const std::string& source, unsigned short line,
											const unsigned short start_index,
											const unsigned short end_index) {
		
		// Find the start and end positions in the string
		size_t start = 0;
		for(size_t i = 1; i < line; i++) {
			start = source.find('\n', start);

			// Move past the newline character
			start++;
		}

		size_t end = source.find('\n', start);
		if(end == std::string::npos)
			end = source.size();

		// Extract the substring
		const std::string lineText = source.substr(start, end - start);


		// Highlight the error
		// const unsigned short offset = (end > source.size()) ? 1 : 0; // +1 after string end, in case error is the string end
		std::string highlight(lineText.size(), ' ');
		for (size_t i = start_index; i <= end_index && i < lineText.size(); i++) {
			highlight[static_cast<size_t>(i)] = '^';
		}

		// Return the result
		return lineText + "\n" + highlight;
	}


	const inline std::string make_error(const Type type, const std::string& detail, const File& file) {
		std::string result;

		// Erro name display
		result += Utils::colorize(errorTypes.at(static_cast<std::array<std::string, 3>::size_type>(type)) + ": ", "bold_red")
				+ Utils::colorize(detail, "green");

		// Error traceback display
		// if(has_context()) result += generate_traceback(get_context());

		result += Utils::colorize("\nFile: ", "gray")
			   +  Utils::colorize(file.filename, "yellow")
			   +  Utils::colorize(" at index position ", "white")
			   +  Utils::colorize(std::to_string(file.index), "blue")
			   +  ", line "
			   +  Utils::colorize(std::to_string(file.index), "blue")
			   +  "\n"

			   // +  highlightError(file.source, file.line, file.index, pos_end.index);
			   +  highlightError(file.source, file.line,
			  	file.index, file.index);

		return result;
	}
}