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

	Command* makeCommand(string& opcode, SsdInterface* ssdInterface, UtilsInterface* utilsInterface) {
		if ("exit" == opcode) { return new ExitCommand(ssdInterface, utilsInterface);}
		else if ("read" == opcode) { return new ReadCommand(ssdInterface, utilsInterface);}
		else if ("write" == opcode) { return new WriteCommand(ssdInterface, utilsInterface);}
		else if ("erase" == opcode) { return new EraseCommand(ssdInterface, utilsInterface); }
		else if ("erase_range" == opcode) { return new EraseRangeCommand(ssdInterface, utilsInterface); }
		else if ("flush" == opcode) { return new FlushCommand(ssdInterface, utilsInterface); }
		else if ("help" == opcode) { return new HelpCommand(ssdInterface, utilsInterface);}
		else if ("fullwrite" == opcode) { return new FullWriteCommand(ssdInterface, utilsInterface);}
		else if ("fullread" == opcode) { return new FullReadCommand(ssdInterface, utilsInterface);}
		else if ("1_FullWriteAndReadCompare" == opcode || "1_" == opcode) { return new FullWriteAndReadCompareCommand(ssdInterface, utilsInterface); }
		else if ("2_PartialLBAWrite" == opcode || "2_" == opcode) { return new PartialLBAWriteCommand(ssdInterface, utilsInterface); }
		else if ("3_WriteReadAging" == opcode || "3_" == opcode) { return new WriteReadAgingCommand(ssdInterface, utilsInterface); }
		else if ("4_EraseAndWriteAging" == opcode || "4_" == opcode) { return new EraseAndWriteAgingCommand(ssdInterface, utilsInterface); }
		// add new command

		return nullptr;
	}
private:
	FactoryCommand() {};
};

bool CommandProcessor::run(vector<string> commands) {
	FactoryCommand factoryCommand = FactoryCommand::getInstance();
	Command* command = factoryCommand.makeCommand(commands[OPCODE], ssdInterface, utilsInterface);

	if (command == nullptr) {
		std::cout << "command is not defined.\n";
		return false;
	}

	return command->run(commands);
}

