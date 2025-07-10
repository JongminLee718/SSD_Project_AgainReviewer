#include "full_write_and_read_compare_command.h"

bool FullWriteAndReadCompareCommand::run(vector<string> commands) {
	for (int i = 0; i < MAX_LBA; i += 4) {
		string randData = utils->genSSDRandData();
		for (int LBA = i; LBA < i + 4; LBA++) {
			ssd->write(to_string(LBA), randData);
			ssd->read(to_string(LBA));

			bool result = utils->outputChecker(randData);
			if (!result) {
				cout << "FAIL\n";
				return false;
			}
		}
	}

	cout << "PASS\n";
	return true;
}
