#include <iostream>
#include <vector>
#include "ssd_client_app.h"
#include "parser/parser.h"
#include "command_processer.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;

void SsdClientApp::startVerify(SsdInterface* sdInterface) {
	Parser parser;
	vector<string> parsedInput = parser.parse(inputCmd);

	CommandProcesser commandProcesser;
	commandProcesser.setSsdInterface(sdInterface);
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
