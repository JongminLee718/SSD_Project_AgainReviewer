#include "command_processer.h"
#include "exit_command.h"
#include "full_read_command.h"
#include "full_write_command.h"
#include "help_command.h"
#include "read_command.h"
#include "write_command.h"


void CommandProcesser::run(std::vector<std::string> commands) {
	std::string command = commands[0];

	if ("exit" == command) {
		ExitCommand exitCommand;
		exitCommand.exit();
	}
	else if ("read" == command) {
		ReadCommand readCommand;
		readCommand.read(0);
	}
	else if ("write" == command) {
		WriteCommand writeCommand;
		writeCommand.write(0,0);
	}
	else if ("help" == command) {
		HelpCommand helpCommand;
		helpCommand.help();
	}
	else if ("fullwrite" == command) {
		FullWriteCommand fullWriteCommand;
		fullWriteCommand.fullWrite(0);
	}
	else if ("fullread" == command) {
		FullReadCommand fullReadCommand;
		fullReadCommand.fullRead();
	}

}