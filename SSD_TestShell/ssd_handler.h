#pragma once

#include "string"

#define interface class
using std::string;

interface SsdInterface {
public:
	virtual void read(string lba);
	virtual void write(string lba, string data);
};

class SsdHandler : public SsdInterface {
public :
	void read(string lba) override;
	void write(string lba, string data) override;
private:
	void runSSD(std::string cmd);
};