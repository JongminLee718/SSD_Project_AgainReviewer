#include "full_write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool FullWriteCommand::run(vector<string> commands) {
	for (int lba = 0; lba < MAX_LBA; lba++) {
		ssdInterface->write(to_string(lba), commands[1]);

		string data = utilsInterface->readOutput();
		if (ERROR_PATERN == data) {
			cout << "[Full Write] ERROR\n";
			return false;;
		}
	}

	cout << "[Full Write] Done\n";
	return true;
}