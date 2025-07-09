#include "full_read_command.h"

#include <iostream>

using std::cout;
using std::to_string;

void FullReadCommand::run(vector<string> commands) {
	cout << "[Full Read]\n";
	
	for (int lba = 0; lba < MAX_LBA; lba++) {
		ssdInterface->read(to_string(lba));

		string data = ssdInterface->readOutput();

		if (ERROR_PATERN == data) {
			cout << "[Full Read] ERROR\n";
			break;
		}
		else {
			cout << "LBA " << std::setfill('0') << std::setw(2) << lba << " : " + data + "\n";
		}
	}
}
