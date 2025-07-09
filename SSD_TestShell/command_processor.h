#pragma once
#include <vector>
#include <string>
#include "command.h"
#include "ssd_handler.h"

using std::vector;
using std::string;

class CommandProcessor {
public:
	CommandProcessor(SsdInterface* ssd, UtilsInterface* utils) : ssdInterface{ ssd }, utilsInterface{ utils } {}
	bool run(vector<string> commands);
private:
	int OPCODE = 0;
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};