#include "write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void WriteCommand::run(vector<string> commands){
	ssdInterface->write(commands[1], commands[2]);
	cout << "[Write] Done\n";
}
