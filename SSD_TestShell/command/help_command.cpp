#include "help_command.h"

bool HelpCommand::run(vector<string> commands) {
	cout << getHelpString();
	return true;
}

string HelpCommand::getHelpString() {
	string result = "";
	result += "Made by Jongmin Lee, Taedong Jung, Lee Jeong Seon, Choi Wonjun, Donghyuck Shin, Jaeyeon Kim\n\n";
	result += "[Command List]\n";
	result += "read : read one LBA.\n";
	result += "write : write one LBA.\n";
	result += "erase : erase up to 10 LBAs.\n";
	result += "erase_range : erase specific LBA range.\n";
	result += "flush : Write the buffer to NAND..\n";
	result += "fullread : read all LBAs.\n";
	result += "fullwrite : write all LBAs.\n";
	result += "exit : finish current shell.\n";
	result += "\n";

	LOG("help cmd");

	return result;
}