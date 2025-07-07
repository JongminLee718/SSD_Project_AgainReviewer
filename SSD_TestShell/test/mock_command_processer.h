#pragma once
#include "gmock/gmock.h"


class MockCommandProcesser {
public:
	void run(std::vector<std::string> commands);
};