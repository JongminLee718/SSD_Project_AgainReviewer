#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class ExitCommand : public Command {
public:
	ExitCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		this->ssdInterface = ssdInterface;
		this->utilsInterface = utilsInterface;
	}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};