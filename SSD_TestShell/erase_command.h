#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class EraseCommand : public Command {
public:
	EraseCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
};