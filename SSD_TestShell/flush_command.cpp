#include "flush_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool FlushCommand::run(vector<string> commands) {
	ssdInterface->flush();

	string data = utilsInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Flush] ERROR\n";
		return false;
	}
	else {
		cout << "[Flush] Done" << "\n";
		return true;
	}
}
