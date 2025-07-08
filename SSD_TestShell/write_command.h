#pragma once

#include "command.h"

class WriteCommand : public Command {
public:
	WriteCommand(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	void run(vector<string> commands) override;
private:
};