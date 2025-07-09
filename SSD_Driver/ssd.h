#pragma once
#include <string>
#include <vector>

class SSD {
public:
	std::vector<unsigned int>& nandData;
	SSD(std::vector<unsigned int>& data) : nandData(data) {}
	std::string doWriteCmd(int address, int data);
	std::string doReadCmd(int address);
	int getData(int addr);
	void setData(int addr, int data);
	void loadData(const std::vector<unsigned int>& memory);
private:
	const int SSD_SIZE = 100;
};
