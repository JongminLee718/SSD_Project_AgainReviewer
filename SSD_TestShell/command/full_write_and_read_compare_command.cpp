#include "full_write_and_read_compare_command.h"

bool FullWriteAndReadCompareCommand::run(vector<string> commands) {
	for (int i = 0; i < MAX_LBA; i += 5) {
		string randData = utils->genSSDRandData();
		for (int LBA = i; LBA < i + 5; LBA++) {
			ssd->write(to_string(LBA), randData);
			ssd->read(to_string(LBA));

			bool result = utils->outputChecker(randData);
			if (!result) {
				LOG("FullWriteAndReadCompareCommand FAIL");
				cout << "FAIL\n";
				return false;
			}
		}
	}
	LOG("FullWriteAndReadCompareCommand PASS");
	cout << "PASS\n";
	return true;
}
