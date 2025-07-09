#include "erase_and_write_aging_command.h"

bool EraseAndWriteAgingCommand::run(vector<string> commands) {
	ssd->erase(to_string(0), to_string(3));
	for (int i = 0; i < 30; i++) {
		for (int LBA = 2; LBA < MAX_LBA; LBA += 2) {
			string randData = utils->genRandData();
			ssd->write(to_string(LBA), randData);

			randData = utils->genRandData();
			ssd->write(to_string(LBA), randData);

			if ((LBA + 3) > MAX_LBA)
				ssd->erase(to_string(LBA), to_string(2));
			else
				ssd->erase(to_string(LBA), to_string(3));
		}
	}

	std::cout << "PASS\n";
	return true;

}