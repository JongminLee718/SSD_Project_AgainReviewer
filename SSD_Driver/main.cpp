#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "main.h"
#include "ssd.h"
#include "fileio.h"

using std::string;
//#define DEBUG_LOG

void writeOutput(const std::string& content) {
	std::ofstream outputFile(OUTPUT_FILE_PATH);
	outputFile << content;
}

void storeNand(SSD* ssd) {
	std::ofstream outputFile(NAND_FILE_PATH);
	for (int addrIdx = 0;addrIdx < SSD_SIZE;addrIdx++) {
		std::stringstream ss;
		ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << ssd->getData(addrIdx)<<"\n";
		outputFile << ss.str();
	}
}

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	
	if (argc < 2) {
		// Not enough arguments
		writeOutput(ERROR);
		return true;
	}

	std::string command = argv[1];
	int addr = std::stoi(argv[2]);
	
	if (command == "R") {
		if (argc != 3) {
			writeOutput(ERROR);
			return true;
		}
		FileInOut fileio(NAND_FILE_PATH);
		SSD handler(fileio.nandData);
		string result = handler.doReadCmd(addr);
		writeOutput(result);
#if defined(DEBUG_LOG)
		std::cout << "R result = " <<addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result << "\n";
#endif
	}
	else if (command == "W") {
		if (argc != 4) {
			writeOutput(ERROR);
			return true;
		}
		string input = argv[3];
		int data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
		FileInOut fileio(NAND_FILE_PATH);
		SSD ssd(fileio.nandData);
		string result = ssd.doWriteCmd(addr, data);
		storeNand(&ssd);
		writeOutput(result);
#if defined(DEBUG_LOG)
		std::cout << "input data = addr" << addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data << "\n";
		std::cout << "W result = " <<  result << "\n";
#endif
	}
	else if (command == "E") {
		if (argc != 4) {
			writeOutput(ERROR);
			return true;
		}
		int size = std::stoi(argv[3]);
		FileInOut fileio(NAND_FILE_PATH);
		SSD ssd(fileio.nandData);
		string result = ssd.doEraseCmd(addr, size);
		storeNand(&ssd);
		writeOutput(result);
#if defined(DEBUG_LOG)
		std::cout << "E result = " << result << "\n";
#endif
	}
	else if (command == "F") {
		if (argc != 2) {
			writeOutput(ERROR);
			return true;
		}

	}
	else {
		writeOutput(ERROR);
	}

	return true;
#endif
}
