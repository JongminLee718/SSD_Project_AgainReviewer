#pragma once

#include "command.h"

class FullWriteCommand : public Command {
public:
	FullWriteCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
};
