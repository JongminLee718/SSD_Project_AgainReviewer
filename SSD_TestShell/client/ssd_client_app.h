#pragma once

#include <string>
#include <vector>
#include "ssd_handler.h"
#include "utils.h"
#include "parser.h"
#include "command_processor.h"

class SsdClientApp {
public:
	SsdClientApp(SsdInterface* ssd, UtilsInterface* utils) : 
		ssdInterface{ ssd }, utilsInterface{ utils }, commandProcesser{ ssdInterface, utilsInterface } {}
	bool startVerify();
	void setInputCmd(std::string input);
	void getUserCmdLine();
	void printError();

private:
	void displayPrompt() const;
	std::vector<string> parseInput();
	std::string readUserInput();	

	SsdInterface* ssdInterface;
	UtilsInterface* utilsInterface;
	CommandProcessor commandProcesser;
	Parser parser;
	std::string inputCmd;
};
