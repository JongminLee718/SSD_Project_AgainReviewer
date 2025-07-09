#pragma once

#include "command.h"
#include "iostream"
#include "string"

using std::cout;
using std::string;

class HelpCommand : public Command {
public:
	HelpCommand(SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		this->ssdInterface = ssdInterface;
		this->utilsInterface = utilsInterface;
	}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
	string getHelpString();
};