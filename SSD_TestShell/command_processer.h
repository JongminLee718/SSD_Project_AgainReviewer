#pragma once
#include <vector>
#include <string>
#include "command.h"
#include "ssd_handler.h"

using std::vector;
using std::string;

class CommandProcesser {
public:
	void run(vector<string> commands);
	void setSsdInterface(SsdInterface* ssdInterface);
private:
	int OPCODE = 0;
	SsdInterface* ssdInterface;
};