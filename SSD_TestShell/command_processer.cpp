#include "command_processer.h"

#include "exit_command.h"
#include "full_read_command.h"
#include "full_write_command.h"
#include "help_command.h"
#include "read_command.h"
#include "write_command.h"

//Simple Factory Pattern
class FactoryCommand {
public:
	static FactoryCommand& getInstance() {
		static FactoryCommand instance;
		return instance;
	}

	Command* makeCommand(string& opcode) {
		if ("exit" == opcode) { return new ExitCommand();}
		else if ("read" == opcode) { return new ReadCommand();}
		else if ("write" == opcode) { return new WriteCommand();}
		else if ("help" == opcode) { return new HelpCommand();}
		else if ("fullwrite" == opcode) { return new FullWriteCommand();}
		else if ("fullread" == opcode) { return new FullReadCommand();}

		return nullptr;
	}
private:
	FactoryCommand() {};
};

void CommandProcesser::run(vector<string> commands) {
	FactoryCommand factoryCommand = FactoryCommand::getInstance();

	Command* command = factoryCommand.makeCommand(commands[OPCODE]);

	command->run(commands);
}

