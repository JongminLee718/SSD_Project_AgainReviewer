#pragma once

#include "command.h"

class FullWriteCommand : public Command {
public:
	FullWriteCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		this->ssdInterface = ssdInterface;
		this->utilsInterface = utilsInterface;
	}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};
