#include "full_read_command.h"

#include <iostream>

using std::cout;
using std::to_string;

bool FullReadCommand::run(vector<string> commands) {
	cout << "[Full Read]\n";
	
	for (int lba = 0; lba < MAX_LBA; lba++) {
		ssdInterface->read(to_string(lba));
		string data = utilsInterface->readOutput();

		if (ERROR_PATERN == data) {
			LOG("[Full Read] ERROR Pattern");
			cout << "[Full Read] ERROR\n";
			return false;;
		}

		cout << "LBA " << std::setfill('0') << std::setw(2) << lba << " : " + data + "\n";
	}

	LOG("[Full Read] Done");
	return true;
}
