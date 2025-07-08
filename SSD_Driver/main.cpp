#include <iostream>
#include <string>
#include "gmock/gmock.h"
#include "ssd.h"

#define RUN_UT

using std::string;

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
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
			int addr = 0, data = 0;
			ssd.doWriteCmd(addr, data);
		}
		else {
			// Incorrect number of arguments for W
		}
	}
	else {
		// Unknown command
	}
#endif
}
