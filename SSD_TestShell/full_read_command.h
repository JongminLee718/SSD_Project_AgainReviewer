#pragma once

#include "command.h"

class FullReadCommand : public Command{
public:
	void run(vector<string> commands) override;
private:
};