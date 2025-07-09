#include "exit_command.h"

bool ExitCommand::run(vector<string> commands) {
	cout << "exit program\n";
	exit(0);
	return true;
}