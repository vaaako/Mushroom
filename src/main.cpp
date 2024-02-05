#include <cstdlib>
#include <fstream>
#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#include "headers/mushroom.hpp"
#include "headers/helpers/utils.hpp"

const bool DEBUG = true;

int main(int argc, const char* argv[]) {
	argv[1] = "test2.mush";

	if(argv[1]) {

		// Read file as binary
		std::ifstream sfile = std::ifstream(argv[1], std::ios::binary);
		
		// Check if exists
		if(!sfile.is_open()) {
			std::cout << "Unable to find file: " + std::string(argv[1]) << std::endl;
			return EXIT_FAILURE;
		}

		// Get content
		const std::string file = Utils::get_file_content(sfile);

		// Run Mushroom
		Mushroom mushroom = Mushroom(argv[1], DEBUG);
		const bool res = mushroom.run(file);

		// There is no reason of doing this
		return (res) ? EXIT_SUCCESS : EXIT_FAILURE;
	}


	// Command line //
	std::cout << "Mushroom v0.0.1 [clang 14.0.0]" << std::endl;
	std::cout << "Press Ctrl + C or type \"exit\" to exit" << std::endl;
	
	std::string input;
	Mushroom mushroom = Mushroom("<stdin>", DEBUG);
	while(true) {
		input = readline("> ");

		if(input == "exit") break;

		// Add input to history
		add_history(input.c_str());

		// Interpreter
		std::cout << input << std::endl;
		mushroom.run(input);
	}
}