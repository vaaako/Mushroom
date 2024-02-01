#pragma once

#include <string>

class Mushroom {
	public:
		Mushroom(const std::string& filename, const bool debug);

		// SUCESS / FAILURE
		bool run(const std::string& source) const;
	private:
		const std::string filename;
		const bool debug;

		// Environment* env;
		// File* file;
};