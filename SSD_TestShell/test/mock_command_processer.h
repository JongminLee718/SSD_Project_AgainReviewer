#pragma once
#include "gmock/gmock.h"
#include "../ssd_handler.h"
#include <string>

using std::string;

class MockSssHandler : public SsdInterface {
public:
	MOCK_METHOD(void, read, (string lba), (override));
	MOCK_METHOD(void, write, (string lba, string data), (override));
	MOCK_METHOD(string, readOutput, (), (override));
};
