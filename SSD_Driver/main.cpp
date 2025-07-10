#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "main.h"
#include "ssd.h"
#include "fileio.h"
#include "bufferManager.h"

using std::string;

#if !defined(_DEBUG)
int writeOutput(const std::string& content) {
	std::ofstream outputFile(OUTPUT_FILE_PATH);
	outputFile << content;
	return true;
}

string getStringFromReadValue(unsigned int readValue) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
}

bool CheckAdressValidation(int address) { return address < 0 || address >= SSD_SIZE; }
bool CheckEraseRangeValidation(int address, int size) { return address + size > SSD_SIZE; }
bool CheckEraseSizeValidation(int size) { return size < 1 || size > 10; }
#endif

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	
	if (argc < 2) return writeOutput(ERROR);

	std::string command = argv[1];
	if (!(command == "R" || command == "W" || command == "E" || command == "F")) writeOutput(ERROR);
	
	if (command == "R") {
		if (argc != 3) return writeOutput(ERROR);
		int addr = std::stoi(argv[2]);
		if (CheckAdressValidation(addr)) return writeOutput(ERROR);

		BufferManager buffer(NAND_FILE_PATH);
		buffer.initializeBuffer();
		unsigned int readValue = 0;
		bool isBufResult = buffer.readFromBuffer(addr, readValue);
		string result;
		if (isBufResult) {
			result = getStringFromReadValue(readValue);
			writeOutput(result);
		}
		else {
			FileInOut fileio(NAND_FILE_PATH);
			SSD handler(fileio.nandData);
			result = handler.doReadCmd(addr);
			writeOutput(result);
		}
	}
	if (command == "W") {
		if (argc != 4) return writeOutput(ERROR);
		int addr = std::stoi(argv[2]);
		if (CheckAdressValidation(addr)) return writeOutput(ERROR);

		BufferManager buffer(NAND_FILE_PATH);
		buffer.initializeBuffer();
		string input = argv[3];
		int data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
		buffer.addCommandInBuffer("W", addr, data);
		string result = PASS;
		writeOutput(result);
	}
	if (command == "E") {
		if (argc != 4) return writeOutput(ERROR);
		int addr = std::stoi(argv[2]);
		int size = std::stoi(argv[3]);
		if (CheckAdressValidation(addr)) return writeOutput(ERROR);
		if (CheckEraseSizeValidation(size)) return writeOutput(ERROR);
		if (CheckEraseRangeValidation(addr, size)) return writeOutput(ERROR);

		BufferManager buffer(NAND_FILE_PATH);
		buffer.initializeBuffer();
		buffer.addCommandInBuffer("E", addr, size);
		string result = PASS;
		writeOutput(result);
	}
	if (command == "F") {
		if (argc != 2) return writeOutput(ERROR);

		BufferManager buffer(NAND_FILE_PATH);
		buffer.initializeBuffer();
		buffer.flush();
	}

	return true;
#endif
}
