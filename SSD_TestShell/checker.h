#pragma once
#include "gmock/gmock.h"
#include <string>
#include <iomanip>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>

using std::string;

class Checker {
public:
	virtual bool outputChecker(const string& data) = 0;
};

class OutputChecker : public Checker {
public:
	bool outputChecker(const string& data) override;
};

class OutputCheckerMock : public Checker {
public:
	MOCK_METHOD(bool, outputChecker, (const string&), (override));
};