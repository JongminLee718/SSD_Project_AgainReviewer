#include "help_command.h"

void HelpCommand::run(vector<string> commands) {
	// print help info
	cout << getHelpString();
}

string HelpCommand::getHelpString() {
	string result = "read : read one LBA.\n";
	result += "write : write one LBA.\n";
	result += "fullread : read all LBAs.\n";
	result += "fullwrite : write all LBAs.\n";
	result += "exit : finish current shell.\n";

	return result;
}