#pragma once

#include "command.h"

class WriteCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};