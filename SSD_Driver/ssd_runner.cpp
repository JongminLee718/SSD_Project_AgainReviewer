#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ssd_runner.h"
#include "main.h"
#include "ssd.h"
#include "fileio.h"
#include "buffer_manager.h"

using std::string;
#if !defined(_DEBUG)
int SsdRunner::readHandler(char* argv[], int argc)
{
	int addr = std::stoi(argv[2]);
	if (argc != 3) return writeOutput(ERROR);
	if (CheckAdressValidation(addr)) return writeOutput(ERROR);
	BufferManager buffer(NAND_FILE_PATH);
	buffer.initializeBuffer();
	unsigned int readValue = 0;
	bool isBufResult = buffer.readFromBuffer(addr, readValue);
	string result;
	if (isBufResult) {
		result = getStringFromReadValue(readValue);
	}
	else {
		FileInOut fileio(NAND_FILE_PATH);
		SSD handler(fileio.nandData);
		result = handler.doReadCmd(addr);
	}
	return writeOutput(result);
}

int SsdRunner::writeHandler(char* argv[], int argc)
{
	int addr = std::stoi(argv[2]);
	int data = getIntDateFromString(argv[3]);
	if (argc != 4) return writeOutput(ERROR);
	if (CheckAdressValidation(addr)) return writeOutput(ERROR);
	BufferManager buffer(NAND_FILE_PATH);
	buffer.initializeBuffer();
	buffer.addCommandInBuffer("W", addr, data);
	return writeOutput(PASS);
}

int SsdRunner::eraseHandler(char* argv[], int argc)
{
	int addr = std::stoi(argv[2]);
	int size = std::stoi(argv[3]);
	if (argc != 4) return writeOutput(ERROR);
	if (CheckAdressValidation(addr)) return writeOutput(ERROR);
	if (CheckEraseSizeValidation(size)) return writeOutput(ERROR);
	if (CheckEraseRangeValidation(addr, size)) return writeOutput(ERROR);
	BufferManager buffer(NAND_FILE_PATH);
	buffer.initializeBuffer();
	buffer.addCommandInBuffer("E", addr, size);
	return writeOutput(PASS);
}

int SsdRunner::flushHandler(int argc)
{
	if (argc != 2) return writeOutput(ERROR);
	BufferManager buffer(NAND_FILE_PATH);
	buffer.initializeBuffer();
	buffer.flush();
	return true;
}

int SsdRunner::writeOutput(const std::string& content) {
	std::ofstream outputFile(OUTPUT_FILE_PATH);
	outputFile << content;
	return true;
}

string SsdRunner::getStringFromReadValue(unsigned int readValue) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
}

int SsdRunner::getIntDateFromString(char* argv)
{
	int data;
	string input = argv;
	data = (input.find("0x") == std::string::npos) ? std::stoll(input, nullptr, 10) : std::stoll(input, nullptr, 16);
	return data;
}
#endif