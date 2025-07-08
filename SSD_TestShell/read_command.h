#pragma once

#include "command.h"

class ReadCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};