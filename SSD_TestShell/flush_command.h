#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class FlushCommand : public Command {
public:
	FlushCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
	const int LBA_OFFSET = 1;
	const int LBA_SIZE_OFFSET = 2;
	SsdInterface* ssdInterface;
};