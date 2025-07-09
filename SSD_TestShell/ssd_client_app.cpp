#include <iostream>
#include <vector>
#include "ssd_client_app.h"
#include "parser/parser.h"
#include "command_processor.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;

void SsdClientApp::startVerify(SsdInterface* sdInterface, Checker* checker) {
	Parser parser;
	vector<string> parsedInput = parser.parse(inputCmd);
	
	CommandProcessor commandProcesser{ sdInterface, checker };
	commandProcesser.run(parsedInput);
}

void SsdClientApp::setInputCmd(string input) {
	inputCmd = input;
}

void SsdClientApp::getUserCmdLine() {
	displayPrompt();
	setInputCmd(readUserInput());
}

void SsdClientApp::displayPrompt() const {
	cout << "Shell> ";
}

string SsdClientApp::readUserInput() {
	string input;
	getline(cin, input);
	return input;
}

void SsdClientApp::printError() {
	string cmdPrint;
	if (inputCmd.find("write") == 0) cmdPrint = "[Write]";
	else if (inputCmd.find("read") == 0) cmdPrint = "[Read]";
	else if (inputCmd.find("fullwrite") == 0) cmdPrint = "[Full Write]";
	else if (inputCmd.find("fullread") == 0) cmdPrint = "[Full Read]";
	cout << cmdPrint << " ERROR\n";
}
