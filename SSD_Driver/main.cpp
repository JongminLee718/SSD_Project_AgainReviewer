#include <iostream>
#include <string>
#include <fstream>
#include "gmock/gmock.h"
#include "ssd.h"
#include "fileio.h"

using std::string;
const std::string NAND_FILE_PATH = "ssd_nand.txt";
const std::string OUTPUT_FILE_PATH = "ssd_output.txt";
const std::string ERROR = "ERROR";

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
	
	if (argc < 2) {
		// Not enough arguments
		std::cout << "argunement error" << "\n";
		return true;
	}

	std::string command = argv[1];
	int addr = std::stoi(argv[2]);
	

	if (command == "R") {
		if (argc == 3) { // R <LBA>
			FileInOut fileio(NAND_FILE_PATH);
			SSD handler(fileio.nandData);
			string result = handler.doReadCmd(addr);
			writeOutput(result);
			std::cout << "R result = " <<addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result << "\n";
		}
		else {
			// Incorrect number of arguments for R
			std::cout << "Incorrect number of arguments for R" << "\n";
			writeOutput(ERROR);
		}
	}
	else if (command == "W") {
		if (argc == 4) { // W <LBA> <VALUE>
			string input = argv[3];
			int data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
			std::cout << "input data = addr" << addr << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data << "\n";
			FileInOut fileio(NAND_FILE_PATH);
			SSD ssd(fileio.nandData);
			string result = ssd.doWriteCmd(addr, data);
			storeNand(&ssd);
			writeOutput(result);
			std::cout << "W result = " <<  result << "\n";
		}
		else {
			std::cout << "Incorrect number of arguments for W" << "\n";
			writeOutput(ERROR);
		}
	}
	else if (command == "E") {
		if (argc == 4) {
			int size = std::stoi(argv[3]);
			FileInOut fileio(NAND_FILE_PATH);
			SSD ssd(fileio.nandData);
			string result = ssd.doEraseCmd(addr, size);
			storeNand(&ssd);
			writeOutput(result);
			std::cout << "E result = " << result << "\n";
		}
		else {
			std::cout << "Incorrect number of arguments for E" << "\n";
			writeOutput(ERROR);
		}
	}
	else if (command == "F") {
		if (argc == 2) {

		}
		else {
			std::cout << "Incorrect number of arguments for F" << "\n";
			writeOutput(ERROR);
		}
	}
	else {
		std::cout << "Incorrect command" << "\n";
		writeOutput(ERROR);
	}

	return true;
#endif
}
