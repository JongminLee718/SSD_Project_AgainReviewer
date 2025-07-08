#pragma once

#include "command.h"
#include "iostream"
#include "string"

using std::cout;
using std::string;

class HelpCommand : public Command {
public:
	void run(vector<string> commands) override;
private:
	string getHelpString();
};