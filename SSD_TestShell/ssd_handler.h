#pragma once
#include "gmock/gmock.h"
#include "string"

#define interface class
using std::string;

interface SsdInterface {
public:
	virtual void read(string lba) = 0;
	virtual void write(string lba, string data) = 0;
};

class SsdHandler : public SsdInterface {
public :
	void read(string lba) override;
	void write(string lba, string data) override;
private:
	void runSSD(std::string cmd);
};

class SsdHandlerMock : public SsdInterface {
public:
	MOCK_METHOD(void, read, (string), (override));
	MOCK_METHOD(void, write, (string, string), (override));
};