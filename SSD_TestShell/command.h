#pragma once

#include <vector>
#include <string>
#include "ssd_handler.h"
#include "utils.h"
#define interface class

using std::vector;
using std::string;

interface Command {
public:
	//Command() = default;
	virtual bool run(vector<string> commands) = 0;

protected:
	const int MAX_LBA = 100;
	string ERROR_PATERN = "ERROR";
};