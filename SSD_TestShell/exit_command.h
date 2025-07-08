#pragma once

#include "command.h"

class ExitCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};