#include "erase_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool EraseCommand::run(vector<string> commands) {
	ssdInterface->erase(commands[LBA_OFFSET], commands[LBA_SIZE_OFFSET]);

	if (ERROR_PATERN == utilsInterface->readOutput()) {
		LOG("[Erase] ERROR");
		cout << "[Erase] ERROR\n";
		return false;
	}
	
	LOG("[Erase] Done");
	cout << "[Erase] Done\n";
	return true;
}
