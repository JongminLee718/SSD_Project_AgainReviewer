#pragma once

#include <string>
#include "ssd_handler.h"
#include "utils.h"

class SsdClientApp {
public:
	SsdClientApp(SsdInterface* ssd, UtilsInterface* utils) : ssdInterface{ ssd }, utilsInterface{ utils } {}
	bool startVerify();
	void setInputCmd(std::string input);
	void getUserCmdLine();
	void printError();

private:
	void displayPrompt() const;
	std::string readUserInput();
	std::string inputCmd;
	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
};
