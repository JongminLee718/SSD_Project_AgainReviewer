#pragma once

#include "command.h"

class ReadCommand : public Command {
public:
	ReadCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) : ssdInterface(ssdInterface), utilsInterface(utilsInterface) {}

	bool run(vector<string> commands) override;
private:
	const int LBA_OFFSET = 1;
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};