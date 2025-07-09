#pragma once
#include <vector>
#include <string>
#include "command.h"
#include "ssd_handler.h"
#include "checker.h"

using std::vector;
using std::string;

class CommandProcessor {
public:
	CommandProcessor(SsdInterface* ssd, Checker* cc) : ssdInterface{ ssd }, checker{ cc } {}
	void run(vector<string> commands);
private:
	int OPCODE = 0;
	SsdInterface* ssdInterface;
	Checker* checker;
};