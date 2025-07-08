#pragma once
#include <string>

using std::string;

class Command {
public:
	virtual void set(const string cmd) = 0;
	virtual bool run() = 0;
};

class Checker {
public:
	virtual bool nandWriteAndReadAndChecker(int LBA, const string& data) = 0;
	virtual bool nandReadAndChecker(int LBA, const string& data) = 0;
	virtual bool nandOutputChecker(string data) = 0;
};