#include "exit_command.h"

bool ExitCommand::run(vector<string> commands) {
	LOG("exit program");
	cout << "exit program\n";
	exit(0);
	return true;
}