#include "command_processor.h"

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

	Command* makeCommand(string& opcode, SsdInterface* ssdInterface) {
		if ("exit" == opcode) { return new ExitCommand(ssdInterface);}
		else if ("read" == opcode) { return new ReadCommand(ssdInterface);}
		else if ("write" == opcode) { return new WriteCommand(ssdInterface);}
		else if ("help" == opcode) { return new HelpCommand(ssdInterface);}
		else if ("fullwrite" == opcode) { return new FullWriteCommand(ssdInterface);}
		else if ("fullread" == opcode) { return new FullReadCommand(ssdInterface);}

		return nullptr;
	}
private:
	FactoryCommand() {};
};

void CommandProcessor::run(vector<string> commands) {
	FactoryCommand factoryCommand = FactoryCommand::getInstance();

	Command* command = factoryCommand.makeCommand(commands[OPCODE], ssdInterface);

	command->run(commands);
}

