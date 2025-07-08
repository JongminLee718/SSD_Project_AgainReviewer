#include "read_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void ReadCommand::run(vector<string> commands){
	ssdInterface->read(commands[1]);

	cout << "[Read] LBA " + commands[1] + " : 0x00000000\n";
}
