#pragma once

#include <vector>
#include <string>

#define interface class

using std::vector;
using std::string;

interface Command {
public:
	virtual void run(vector<string> commands) = 0;

protected:
	const int MAX_LBA = 100;
};