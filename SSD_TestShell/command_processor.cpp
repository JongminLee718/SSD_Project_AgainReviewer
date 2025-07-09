#include "command_processor.h"

#include "exit_command.h"
#include "full_read_command.h"
#include "full_write_command.h"
#include "help_command.h"
#include "read_command.h"
#include "write_command.h"
#include "erase_command.h"
#include "flush_command.h"
#include "erase_range_command.h"

#include "full_write_and_read_compare_command.h"
#include "partial_lba_write_command.h"
#include "write_read_aging_command.h"
#include "erase_and_write_aging_command.h"
#include "iostream"

class FactoryCommand {
public:
	static FactoryCommand& getInstance() {
		static FactoryCommand instance;
		return instance;
	}

	Command* makeCommand(string& opcode, SsdInterface* ssdInterface, Checker* checker) {
		if ("exit" == opcode) { return new ExitCommand(ssdInterface);}
		else if ("read" == opcode) { return new ReadCommand(ssdInterface);}
		else if ("write" == opcode) { return new WriteCommand(ssdInterface);}
		else if ("erase" == opcode) { return new EraseCommand(ssdInterface); }
		else if ("erase_range" == opcode) { return new EraseRangeCommand(ssdInterface); }
		else if ("flush" == opcode) { return new FlushCommand(ssdInterface); }
		else if ("help" == opcode) { return new HelpCommand(ssdInterface);}
		else if ("fullwrite" == opcode) { return new FullWriteCommand(ssdInterface);}
		else if ("fullread" == opcode) { return new FullReadCommand(ssdInterface);}
		else if ("1_FullWriteAndReadCompare" == opcode || "1_" == opcode) { return new FullWriteAndReadCompareCommand(ssdInterface, checker); }
		else if ("2_PartialLBAWrite" == opcode || "2_" == opcode) { return new PartialLBAWriteCommand(ssdInterface, checker); }
		else if ("3_WriteReadAging" == opcode || "3_" == opcode) { return new WriteReadAgingCommand(ssdInterface, checker); }
		else if ("4_EraseAndWriteAging" == opcode || "4_" == opcode) { return new EraseAndWriteAgingCommand(ssdInterface); }
		// add new command

		return nullptr;
	}
private:
	FactoryCommand() {};
};

void CommandProcessor::run(vector<string> commands) {
	FactoryCommand factoryCommand = FactoryCommand::getInstance();

	Command* command = factoryCommand.makeCommand(commands[OPCODE], ssdInterface, checker);

	if (command == nullptr) {
		std::cout << "command is not defined.\n";
		return;
	}
	command->run(commands);
}

