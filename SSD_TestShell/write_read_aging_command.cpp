#include "write_read_aging_command.h"

bool WriteReadAgingCommand::run(vector<string> commands) {
	for (int i = 0; i < 200; i++) {
		string randData = utils->genSSDRandData();

		ssd->write(to_string(0), randData);
		ssd->write(to_string(99), randData);

		ssd->read(to_string(0));
		bool result = utils->outputChecker(randData);
		if (!result) {
			LOG("WriteReadAgingCommand 0 FAIL");
			cout << "FAIL\n";
			return false;
		}

		ssd->read(to_string(99));
		result = utils->outputChecker(randData);
		if (!result) {
			LOG("WriteReadAgingCommand 99 FAIL");
			cout << "FAIL\n";
			return false;
		}
	}

	LOG("WriteReadAgingCommand PASS");
	cout << "PASS\n";
	return true;
}
