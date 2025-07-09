#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "main.h"
#include "ssd.h"

using std::string;

string SSD::doWriteCmd(int address, int data) {
	if (CheckAdressValidation(address)) return ERROR;
	setData(address, data);
	return PASS;
}

string SSD::doReadCmd(int address) {
	if (CheckAdressValidation(address)) return ERROR;
	return getStringFromReadValue(getData(address));
}

string SSD::doEraseCmd(int address, int size) {
	if (CheckAdressValidation(address)) return ERROR;
	if (CheckEraseSizeValidation(size)) { return ERROR; }
	if (CheckEraseRangeValidation(address, size)) { return ERROR; }
	for (int addrIdx = 0; addrIdx < size; addrIdx++) {
		setData(address + addrIdx, 0);
	}
	return PASS;
}

string SSD::getStringFromReadValue(unsigned int readValue) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
}

unsigned int SSD::getData(int addr){
	return nandData[addr];
}

void SSD::setData(int addr, int data) {
	nandData[addr] = data;
}

void SSD::loadData(const std::vector<unsigned int> &memory) {
	for (int idx = 0; idx < SSD_SIZE; idx++) {
		nandData[idx] = memory[idx];
	}
}