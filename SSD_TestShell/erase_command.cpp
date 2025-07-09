#include "erase_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void EraseCommand::run(vector<string> commands) {
	ssdInterface->erase(commands[LBA_OFFSET], commands[LBA_SIZE_OFFSET]);

	string data = utilsInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Erase] ERROR\n";
	}
	else {
		cout << "[Erase] Done\n";
	}
}
