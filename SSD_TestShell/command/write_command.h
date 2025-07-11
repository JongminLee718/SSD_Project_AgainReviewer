#pragma once

#include "command.h"

class WriteCommand : public Command {
public:
	WriteCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		this->ssdInterface = ssdInterface;
		this->utilsInterface = utilsInterface;
	}
	bool run(vector<string> commands) override;
private:
	const int LBA_OFFSET = 1;
	const int DATA_OFFSET = 2;

	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};