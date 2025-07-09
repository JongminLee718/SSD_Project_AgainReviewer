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
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};