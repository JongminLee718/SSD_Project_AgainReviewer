#include "full_write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool FullWriteCommand::run(vector<string> commands) {
	for (int lba = 0; lba < MAX_LBA; lba++) {
		ssdInterface->write(to_string(lba), commands[1]);

		if (ERROR_PATERN == utilsInterface->readOutput()) {
			LOG("[Full Write] ERROR Pattern");
			cout << "[Full Write] ERROR\n";
			return false;;
		}
	}

	LOG("[Full Write] Done");
	cout << "[Full Write] Done\n";
	return true;
}