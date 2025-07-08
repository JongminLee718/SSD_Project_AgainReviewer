#pragma once
#include "gmock/gmock.h"
#include "shell_command.h"

class CommandMock : public Command {
public:
	MOCK_METHOD(void, set, (const string cmd), (override));
	MOCK_METHOD(bool, run, (), (override));
};

class CheckerMock : public Checker{
public:
	MOCK_METHOD(bool, nandWriteAndReadAndChecker, (int, const string&), (override));
	MOCK_METHOD(bool, nandReadAndChecker, (int, const string&), (override));
	MOCK_METHOD(bool, nandOutputChecker, (string), (override));
};