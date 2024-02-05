#include "../headers/helpers/file.hpp"

File::File(const std::string& filename, const std::string& source,
					 const unsigned short index, const unsigned short line, const unsigned short col)
	: filename(filename), source(source), index(index), line(line), col(col) {}

void File::advance(const char ch) {
	index++;
	col++;

	// Next line
	if(ch == '\n') {
		line++;
		index = 0; // Reset index on changing line
		col = 0;
	}
}