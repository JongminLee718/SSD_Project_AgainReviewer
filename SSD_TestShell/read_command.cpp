#include "read_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void ReadCommand::read(int lba) {
	cout << "[Read] LBA " + to_string(lba) + " : 0x00000000\n";
	// SSD Interface read
}
