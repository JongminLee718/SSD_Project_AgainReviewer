#include "write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void WriteCommand::run(vector<string> commands){
	ssdInterface->write(commands[1], commands[2]);
	string data = utilsInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Write] ERROR\n";
	}
	else {
		cout << "[Write] Done\n";
	}
}
