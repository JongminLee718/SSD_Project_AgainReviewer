#include "partial_lba_write_command.h"

bool PartialLBAWriteCommand::run(vector<string> commands) {
	for (int i = 0; i < 30; i++) {
		string randData = utils->genSSDRandData();

		ssd->write(to_string(4), randData);
		ssd->write(to_string(0), randData);
		ssd->write(to_string(3), randData);
		ssd->write(to_string(1), randData);
		ssd->write(to_string(2), randData);

		for (int LBA = 0; LBA < 5; LBA++) {
			ssd->read(to_string(LBA));

			bool result = utils->outputChecker(randData);
			if (!result) {
				LOG("PartialLBAWriteCommand FAIL");
				cout << "FAIL\n";
				return false;
			}
		}
	}

	LOG("PartialLBAWriteCommand PASS");
	cout << "PASS\n";
	return true;
}
