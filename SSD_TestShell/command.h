#pragma once

#include <vector>
#include <string>
#include "ssd_handler.h"
#define interface class

using std::vector;
using std::string;

interface Command {
public:
	Command() = default;
	Command(SsdInterface* ssdInterface) {
		this->ssdInterface = ssdInterface;
	}
	virtual void run(vector<string> commands) = 0;

protected:
	const int MAX_LBA = 100;
	SsdInterface* ssdInterface;
};