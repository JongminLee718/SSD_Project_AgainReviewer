#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class ExitCommand : public Command {
public:
	ExitCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
};