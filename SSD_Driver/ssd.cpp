#include "ssd.h"

bool SSD::doWriteCmd(int address, int data) {
	if (address < 0 || address >= 100) return false;
	m_ssd[address] = data;
	return true;
}

void SSD::doReadCmd() {

	return;
}