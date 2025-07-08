#include "read_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void ReadCommand::run(vector<string> commands){
	ssdInterface->read(commands[1]);

	string data = ssdInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Read] ERROR\n";
	}
	else {
		cout << "[Read] LBA " + commands[1] + " : "+ data +"\n";
	}
}
