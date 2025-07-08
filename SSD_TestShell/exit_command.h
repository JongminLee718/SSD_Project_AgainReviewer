#pragma once

#include "command.h"
#include "iostream"

using std::cout;

class ExitCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
};