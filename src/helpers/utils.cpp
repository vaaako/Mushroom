#include "../headers/helpers/utils.hpp"

#include <fstream>
#include <sstream>

std::string Utils::get_file_content(std::ifstream& file) {
	std::stringstream buffer;
	buffer << file.rdbuf(); // Read content
	file.close();

	return buffer.str();
}