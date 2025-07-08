#pragma once
#include <vector>
#include <string>
#include "command.h"

using std::vector;
using std::string;

class CommandProcesser {
public:
	void run(vector<string> commands);

private:
	int OPCODE = 0;

	Command* getCommand(string& opcode);
};