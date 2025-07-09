#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "main.h"
#include "ssd.h"
#include "fileio.h"

#include "bufferManager.cpp"

using std::string;
#define DEBUG_LOG

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

string getStringFromReadValue(unsigned int readValue) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
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

	if (!(command == "R" || command == "W" || command == "E" || command == "F")) writeOutput(ERROR);
	
	if (command == "R") {
		if (argc != 3) {
			writeOutput(ERROR);
			return true;
		}
		BufferManager buffer;
		buffer.initializeBuffer();
		unsigned int readValue = 0;
		bool isBufResult = buffer.readFromBuffer(addr, readValue);
		string result;
		if (isBufResult) {
			result = getStringFromReadValue(readValue);
			writeOutput(result);
			std::cout << "R result from buffer = " << addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result << "\n";
		}
		else {
			FileInOut fileio(NAND_FILE_PATH);
			SSD handler(fileio.nandData);
			result = handler.doReadCmd(addr);
			writeOutput(result);
		}

#if defined(DEBUG_LOG)
		std::cout << "R result = " <<addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result << "\n";
#endif
	}
	if (command == "W") {
		if (argc != 4) {
			writeOutput(ERROR);
			return true;
		}
		BufferManager buffer;
		buffer.initializeBuffer();
		string input = argv[3];
		int data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
		buffer.addCommandInBuffer("W", addr, data);
		string result = PASS;
		writeOutput(result);
#if RESERVED_BUF
		FileInOut fileio(NAND_FILE_PATH);
		SSD ssd(fileio.nandData);
		string result = ssd.doWriteCmd(addr, data);
		storeNand(&ssd);
		writeOutput(result);
#endif
#if defined(DEBUG_LOG)
		std::cout << "input data = addr" << addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data << "\n";
		std::cout << "W result = " <<  result << "\n";
#endif
	}
	if (command == "E") {
		if (argc != 4) {
			writeOutput(ERROR);
			return true;
		}
		BufferManager buffer;
		buffer.initializeBuffer();

		int size = std::stoi(argv[3]);
		buffer.addCommandInBuffer("E", addr, size);
		string result = PASS;
		writeOutput(result);
#if RESERVED_BUF
		FileInOut fileio(NAND_FILE_PATH);
		SSD ssd(fileio.nandData);
		string result = ssd.doEraseCmd(addr, size);
		storeNand(&ssd);
		writeOutput(result);
#endif
#if defined(DEBUG_LOG)
		std::cout << "E result = " << result << "\n";
#endif
	}
	if (command == "F") {
		if (argc != 2) {
			writeOutput(ERROR);
			return true;
		}

	}

	return true;
#endif
}
