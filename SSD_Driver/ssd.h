#pragma once
#include <string>
#include <vector>
#include "main.h"

class SSD {
public:
	std::vector<unsigned int>& nandData;
	SSD(std::vector<unsigned int>& data) : nandData(data) {}
	std::string doWriteCmd(int address, int data);
	std::string doReadCmd(int address);
	std::string doEraseCmd(int address, int size);
	std::string getStringFromReadValue(unsigned int readValue);
	unsigned int getData(int addr);
	void setData(int addr, int data);
	void storeNand();
private:
	bool CheckAdressValidation(int address) { return address < 0 || address >= SSD_SIZE; }
	bool CheckEraseRangeValidation(int address, int size) { return address + size > SSD_SIZE; }
	bool CheckEraseSizeValidation(int size) { return size < 1 || size > 10; }
};
