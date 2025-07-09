#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class EraseRangeCommand : public Command {
public:
	EraseRangeCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;

	
private:
	const int LBA_OFFSET = 1;
	const int LBA_SIZE_OFFSET = 2;
	const int MAX_ERASE_RANGE = 10;
	SsdInterface* ssdInterface;

	int changeLbaToInt(string lba);
	int getLbaSize(int startLba, int endLba);

};