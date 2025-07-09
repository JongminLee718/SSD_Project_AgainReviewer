#pragma once

#include <string>
#include "ssd_handler.h"
#include "checker.h"

class SsdClientApp {
public:
	void startVerify(SsdInterface* sdInterface, Checker* checker);
	void setInputCmd(std::string input);
	void getUserCmdLine();
	void printError();

private:
	void displayPrompt() const;
	std::string readUserInput();
	std::string inputCmd;
};
