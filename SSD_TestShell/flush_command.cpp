#include "flush_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool FlushCommand::run(vector<string> commands) {
	ssdInterface->flush();

	if (ERROR_PATERN == utilsInterface->readOutput()) {
		cout << "[Flush] ERROR\n";
		return false;
	}

	cout << "[Flush] Done" << "\n";
	return true;
}
