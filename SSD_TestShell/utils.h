#pragma once
#include "gmock/gmock.h"
#include <string>
#include <random>
#include <sstream>
#include <fstream>

using std::string;
using std::to_string;

#define interface class

interface UtilsInterface {
public:
	virtual string readOutput() = 0;
	virtual bool outputChecker(const string& data) = 0;
	virtual string genRandData() = 0;
};

class Utils : public UtilsInterface {
public:
	Utils();
	string readOutput() override;
	bool outputChecker(const string& data) override;
	string genRandData() override;
private:
	std::mt19937 gen;
	std::uniform_int_distribution<uint32_t> dist;
	std::ostringstream oss;
};

class UtilsMock : public UtilsInterface {
public:
	MOCK_METHOD(string, readOutput, (), (override));
	MOCK_METHOD(bool, outputChecker, (const string&), (override));
	MOCK_METHOD(string, genRandData, (), (override));
};