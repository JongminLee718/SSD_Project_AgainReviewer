#include "flush_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void FlushCommand::run(vector<string> commands) {
	ssdInterface->flush();

	string data = ssdInterface->readOutput();

	if (ERROR_PATERN == data) {
		cout << "[Flush] ERROR\n";
	}
	else {
		cout << "[Flush] Done" << "\n";
	}
}
