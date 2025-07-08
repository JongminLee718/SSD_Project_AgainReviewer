#include "write_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

void WriteCommand::write(int lba, int data) {
	cout << "[Write] Done\n";
	// SSD Interface write
}
