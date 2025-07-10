#pragma once
#include "main.h"

class SsdRunner {
public:
	int readHandler(char* argv[], int argc);
	int writeHandler(char* argv[], int argc);
	int eraseHandler(char* argv[], int argc);
	int flushHandler(int argc);
	int writeOutput(const std::string& content);
private:
	std::string getStringFromReadValue(unsigned int readValue);
	int getIntDateFromString(char* argv);
	bool CheckAdressValidation(int address) { return address < 0 || address >= SSD_SIZE; }
	bool CheckEraseRangeValidation(int address, int size) { return address + size > SSD_SIZE; }
	bool CheckEraseSizeValidation(int size) { return size < 1 || size > 10; }
};