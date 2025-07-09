#pragma once

#include <string>
#include "ssd_handler.h"
#include "utils.h"

class SsdClientApp {
public:
	bool startVerify(SsdInterface* sdInterface, UtilsInterface* utilsInterface);
	void setInputCmd(std::string input);
	void getUserCmdLine();
	void printError();

private:
	void displayPrompt() const;
	std::string readUserInput();
	std::string inputCmd;
};
