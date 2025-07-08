#pragma once
#include "gmock/gmock.h"
#include "shell_command.h"

class CommandMock : public Command {
public:
	MOCK_METHOD(void, set, (const string cmd), (override));
	MOCK_METHOD(bool, run, (), (override));
};
