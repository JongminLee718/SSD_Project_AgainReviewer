#pragma once

#include <vector>
#include <string>
#include "ssd_handler.h"
#define interface class

using std::vector;
using std::string;

interface Command {
public:
	//Command() = default;
	virtual void run(vector<string> commands) = 0;

protected:
	const int MAX_LBA = 100;
	
};