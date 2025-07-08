#include "ssd.h"

bool SSD::doWriteCmd(int address, int data) {
	if (address < 0 || address >= MAX_LBA) return false;
	setData(address, data);
	return true;
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