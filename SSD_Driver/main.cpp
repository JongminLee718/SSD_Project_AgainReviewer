#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "ssd.h"
#include "ssdHandler.h"

#define RUN_UT

using std::string;
const std::string NAND_FILE_PATH = "ssd_nand.txt";
const std::string OUTPUT_FILE_PATH = "ssd_output.txt";

void writeOutput(const std::string& content) {
	std::ofstream outputFile(OUTPUT_FILE_PATH);
	outputFile << content;
}

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
	int addr = std::stoi(argv[2]);

	if (command == "R") {
		if (argc == 3) { // R <LBA>
			SSDHandler ssd_reader(NAND_FILE_PATH);
			string result = ssd_reader.executeRead(addr);
			writeOutput(result);
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
