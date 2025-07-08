#include "ssd.h"

using std::string;

string SSD::doWriteCmd(int address, int data) {
	if (address < 0 || address >= MAX_LBA) return "ERROR";
	setData(address, data);
	return "";
}

void SSD::doReadCmd() {

	return;
}

int SSD::getData(int addr){
	return m_ssd[addr];
}

void SSD::setData(int addr, int data) {
	m_ssd[addr] = data;
}

void SSD::loadData(const std::vector<unsigned int> &memory) {
	for (int idx = 0; idx < 100; idx++) {
		m_ssd[idx] = memory[idx];
	}
}