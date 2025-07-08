#include "full_read_command.h"

#include <iostream>

using std::cout;
using std::to_string;

void FullReadCommand::run(vector<string> commands) {
	for (int lba = 0; lba < 100; lba++) {
		cout << "[Read] LBA " + to_string(lba) + " : 0x00000000\n";
	}
	// SSD Interface Read
}
