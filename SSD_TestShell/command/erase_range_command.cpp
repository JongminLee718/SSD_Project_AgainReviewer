#include "erase_range_command.h"

#include <iostream>
#include <string>

using std::cout;
using std::to_string;

bool EraseRangeCommand::run(vector<string> commands) {
	int startLba = changeLbaToInt(commands[LBA_OFFSET]);
	int endLba = changeLbaToInt(commands[END_LBA_OFFSET]) + 1;

	if (isInvalidRange(startLba, endLba)) {
		LOG("[EraseRange] invalid range. startLba : "+to_string(startLba) + ", endlba : " + to_string(endLba));
		cout << "[EraseRange] ERROR\n";
		return false;
	}

	for (; startLba < endLba;) {
		int lbaSize = getLbaSize(startLba, endLba);

		ssdInterface->erase(to_string(startLba), to_string(lbaSize));

		if (ERROR_PATERN == utilsInterface->readOutput()) {
			LOG("[EraseRange] ERROR pattern");
			cout << "[EraseRange] ERROR\n";
			return false;
		}

		startLba += lbaSize;
	}

	LOG("[EraseRange] Done");
	cout << "[EraseRange] Done\n";
	return true;
}

bool EraseRangeCommand::isInvalidRange(int startLba, int endLba)
{
	return startLba > endLba || endLba > MAX_LBA;
}

int  EraseRangeCommand::getLbaSize(int startLba, int endLba)
{
	if (startLba + MAX_ERASE_RANGE > endLba) {
		return endLba - startLba;
	}
	return MAX_ERASE_RANGE;
}

int EraseRangeCommand::changeLbaToInt(string lba)
{
	if (lba.find("0x") != std::string::npos) {
		return std::stoi(lba, nullptr, 16);
	}
	else {
		return  std::stoi(lba);
	}
}
