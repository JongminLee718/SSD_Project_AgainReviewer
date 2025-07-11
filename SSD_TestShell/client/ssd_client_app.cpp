#include <iostream>
#include <vector>
#include "ssd_client_app.h"
#include "logger.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;

bool SsdClientApp::startVerify() {
	LOG("ssd verify");

	if (commandProcesser.run(parseInput())) {
		LOG("SUCESS - ssd verify");
		return true;
	}
	LOG("FAIL- ssd verify");
	return false;
}

vector<string> SsdClientApp::parseInput() {
	return parser.parse(inputCmd);
}

void SsdClientApp::setInputCmd(string input) {
	LOG("user input cmd setting");
	inputCmd = input;
}

void SsdClientApp::getUserCmdLine() {
	LOG("user cmd input");
	displayPrompt();
	setInputCmd(readUserInput());
}

void SsdClientApp::displayPrompt() const {
	cout << "Shell> ";
}

string SsdClientApp::readUserInput() {
	LOG("user cmd input");
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
