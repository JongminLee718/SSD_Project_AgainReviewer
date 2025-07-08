#pragma once

#include <string>
#include "ssd_handler.h"

class SsdClientApp {
public:
	void startVerify(SsdInterface* sdInterface);
	void setInputCmd(std::string input);
	void getUserCmdLine();

private:
	void displayPrompt() const;
	std::string readUserInput();
	std::string inputCmd;
};
