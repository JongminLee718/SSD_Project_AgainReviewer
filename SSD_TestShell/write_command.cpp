#include "write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool WriteCommand::run(vector<string> commands){
	ssdInterface->write(commands[LBA_OFFSET], commands[DATA_OFFSET]);

	if (ERROR_PATERN == utilsInterface->readOutput()) {
		cout << "[Write] ERROR\n";
		return false;
	}

	cout << "[Write] Done\n";
	return true;
}
