#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "ssd.h"
#include "ssdHandler.h"

using std::string;
const std::string NAND_FILE_PATH = "ssd_nand.txt";
const std::string OUTPUT_FILE_PATH = "ssd_output.txt";

void writeOutput(const std::string& content) {
	std::ofstream outputFile(OUTPUT_FILE_PATH);
	outputFile << content;
}

void storeNand(SSD* ssd) {
	std::ofstream outputFile(NAND_FILE_PATH);
	for (int i = 0;i < 100;i++) {
		//std::string ss;
		std::stringstream ss;
		//ss = "0x" + std::to_string(ssd->getData(i));
		ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << ssd->getData(i)<<"\n";
		outputFile << ss.str();
	}
}

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	SSD ssd;
	if (argc < 2) {
		// Not enough arguments
		std::cout << "argunement error" << "\n";
		return 1;
	}

	std::string command = argv[1];
	int addr = std::stoi(argv[2]);
	

	if (command == "R") {
		if (argc == 3) { // R <LBA>
			SSDHandler ssd_reader(NAND_FILE_PATH);
			string result = ssd_reader.executeRead(addr);
			writeOutput(result);
			std::cout << "result = " << result << "\n";
		}
		else {
			// Incorrect number of arguments for R
			std::cout << "Incorrect number of arguments for R" << "\n";
		}
	}
	else if (command == "W") {
		if (argc == 4) { // W <LBA> <VALUE>
			string input = argv[3];
			int data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
			std::cout << "data = " << data << "\n";
			SSDHandler ssd_reader(NAND_FILE_PATH);
			ssd.loadData(ssd_reader.nandData);
			string result = ssd.doWriteCmd(addr, data);
			storeNand(&ssd);
			writeOutput(result);
			std::cout << "result = " << result << "\n";
		}
		else {
			std::cout << "Incorrect write command" << "\n";
		}
	}
	else {
		std::cout << "Incorrect command" << "\n";
	}

	return 1;
#endif
}
