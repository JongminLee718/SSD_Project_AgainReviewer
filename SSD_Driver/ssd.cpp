#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "ssd.h"

using std::string;
const string PASS = "";

string SSD::doWriteCmd(int address, int data) {
	if (address < 0 || address >= SSD_SIZE) { return "ERROR"; }
	setData(address, data);
	return PASS;
}

string SSD::doReadCmd(int address) {
	if (address < 0 || address >= SSD_SIZE) { return "ERROR"; }
	unsigned int readValue = nandData[address];
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
	return ss.str();
}

string SSD::doEraseCmd(int address, int size) {
	if (address < 0 || address >= SSD_SIZE) { return "ERROR"; }
	if (size < 1 || size > 10) { return "ERROR"; }
	if (address + size > SSD_SIZE) { return "ERROR"; }
	for (int addrIdx = 0; addrIdx < size; addrIdx++) {
		setData(address + addrIdx, 0);
	}
	return PASS;
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