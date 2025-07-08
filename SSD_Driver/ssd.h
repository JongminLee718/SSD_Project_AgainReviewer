#pragma once
#include <string>
#include <vector>

class SSD {
public:
	std::string doWriteCmd(int address, int data);
	void doReadCmd();
	int getData(int addr);
	void setData(int addr, int data);
	void loadData(const std::vector<unsigned int>& memory);
private:
	static const int MAX_LBA = 100;
	int m_ssd[MAX_LBA];
};
