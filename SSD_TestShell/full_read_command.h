#pragma once

#include "command.h"

class FullReadCommand : public Command{
public:
	FullReadCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssdInterface;
};