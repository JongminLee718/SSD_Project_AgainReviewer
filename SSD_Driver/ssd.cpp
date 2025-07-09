#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "ssd.h"

using std::string;

string SSD::doWriteCmd(int address, int data) {
	if (address < 0 || address >= SSD_SIZE) { return "ERROR"; }
	setData(address, data);
	return "";
}

string SSD::doReadCmd(int address) {
	if (address < 0 || address >= SSD_SIZE) { return "ERROR"; }
	unsigned int readValue = nandData[address];
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
}

int SSD::getData(int addr){
	return nandData[addr];
}

void SSD::setData(int addr, int data) {
	nandData[addr] = data;
}

void SSD::loadData(const std::vector<unsigned int> &memory) {
	for (int idx = 0; idx < 100; idx++) {
		nandData[idx] = memory[idx];
	}
}