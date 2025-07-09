#include "read_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool ReadCommand::run(vector<string> commands){
	ssdInterface->read(commands[LBA_OFFSET]);
	string data = utilsInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Read] ERROR\n";
		return false;
	}

	cout << "[Read] LBA " << std::setfill('0') << std::setw(2) << std::stoi(commands[LBA_OFFSET]) << " : "+ data +"\n";
	
	return true;
}
