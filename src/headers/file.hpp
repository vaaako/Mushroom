#pragma once

#include <string>

// Track file position and source
struct File {
	const std::string filename;
	std::string source;

	unsigned short index;
	unsigned short line;
	unsigned short col;

	File(const std::string& filename, const std::string& source,
			  const unsigned short index, const unsigned short line, const unsigned short col);
	void advance(char ch='\0');
};