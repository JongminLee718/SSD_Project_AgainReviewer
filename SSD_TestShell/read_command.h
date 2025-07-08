#pragma once

#include "command.h"

class ReadCommand : public Command {
public:
	ReadCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
};