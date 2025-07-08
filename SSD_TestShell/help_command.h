#pragma once

#include "command.h"

class HelpCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};