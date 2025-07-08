#pragma once
#include <string>

using std::string;

class Command {
public:
	virtual void set(const string cmd) = 0;
	virtual bool run() = 0;
};