#include <iostream>
#include <string>
#include "ssd.h"

using std::string;

int main(int argc, char* argv[]) {
	SSD ssd;
	if (argc < 2) {
		// Not enough arguments
		return 1;
	}

	std::string command = argv[1];

	if (command == "R") {
		if (argc == 3) { // R <LBA>
			ssd.doReadCmd();
		}
		else {
			// Incorrect number of arguments for R
		}
	}
	else if (command == "W") {
		if (argc == 4) { // W <LBA> <VALUE>
			ssd.doWriteCmd();
		}
		else {
			// Incorrect number of arguments for W
		}
	}
	else {
		// Unknown command
	}
}

