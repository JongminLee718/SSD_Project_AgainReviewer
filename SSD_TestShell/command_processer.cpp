#include "command_processer.h"

#include "exit_command.h"
#include "full_read_command.h"
#include "full_write_command.h"
#include "help_command.h"
#include "read_command.h"
#include "write_command.h"

void CommandProcesser::run(vector<string> commands) {
	Command* command = getCommand(commands[OPCODE]);

	command->run(commands);
}

Command* CommandProcesser::getCommand(string& opcode) {
	Command* command = nullptr;

	if ("exit" == opcode) {
		command = new ExitCommand();
	}
	else if ("read" == opcode) {
		command = new ReadCommand();
	}
	else if ("write" == opcode) {
		command = new WriteCommand();;
	}
	else if ("help" == opcode) {
		command = new HelpCommand();
	}
	else if ("fullwrite" == opcode) {
		command = new FullWriteCommand();
	}
	else if ("fullread" == opcode) {
		command = new FullReadCommand();
	}

	return command;
}