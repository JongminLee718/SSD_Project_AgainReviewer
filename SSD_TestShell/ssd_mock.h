#pragma once
#include "gmock/gmock.h"

#define uint32_t unsigned int

class SSDMockShell {
public:
	MOCK_METHOD(void, R, (const uint32_t &addr), ());
	MOCK_METHOD(void, W, (const uint32_t &addr, const uint32_t &data), ());
};