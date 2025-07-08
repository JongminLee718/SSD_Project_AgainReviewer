#include <iostream>
#include <vector>
#include "ssd_verify_app.h"
#include "parser/parser.h"
#include "command_processer.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;

void SsdVerifyApp::startVerify(SsdInterface* sdInterface) {
	Parser parser;
	vector<string> parsedInput = parser.parse(inputCmd);

	CommandProcesser commandProcesser;
	commandProcesser.setSsdInterface(sdInterface);
	commandProcesser.run(parsedInput);
}

void SsdVerifyApp::setInputCmd(string input) {
	inputCmd = input;
}

void SsdVerifyApp::getUserCmdLine() {
	cout << "Shell> ";
	string input;
	getline(cin, input);
	setInputCmd(input);
}