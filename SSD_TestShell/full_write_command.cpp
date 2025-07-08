#include "full_write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void FullWriteCommand::run(vector<string> commands) {
	for (int lba = 0; lba < MAX_LBA; lba++) {
		ssdInterface->write(to_string(lba), commands[1]);

		string data = ssdInterface->readOutput();
		if (ERROR_PATERN == data) {
			cout << "[Write] ERROR\n";
			return;
		}
	}

	cout << "[Write] Done\n";
}