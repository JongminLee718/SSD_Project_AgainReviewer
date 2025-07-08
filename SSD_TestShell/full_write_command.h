#pragma once

#include "command.h"

class FullWriteCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};
