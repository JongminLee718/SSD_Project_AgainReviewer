#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class FlushCommand : public Command {
public:
	FlushCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		this->ssdInterface = ssdInterface;
		this->utilsInterface = utilsInterface;
	}
	void run(vector<string> commands) override;
private:
	const int LBA_OFFSET = 1;
	const int LBA_SIZE_OFFSET = 2;
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};