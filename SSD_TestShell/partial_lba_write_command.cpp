#include "partial_lba_write_command.h"

using std::cout;

bool PartialLBAWriteCommand::run(vector<string> commands) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
	std::ostringstream oss;

	for (int i = 0; i < 30; i++) {
		uint32_t value = dist(gen);
		oss.str("");
		oss.clear();
		oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
		string randData = oss.str();

		ssd->write(std::to_string(4), randData);
		ssd->write(std::to_string(0), randData);
		ssd->write(std::to_string(3), randData);
		ssd->write(std::to_string(1), randData);
		ssd->write(std::to_string(2), randData);

		for (int LBA = 0; LBA < 5; LBA++) {
			ssd->read(std::to_string(LBA));

			bool result = utils->outputChecker(randData);
			if (!result) {
				std::cout << "FAIL\n";
				return false;
			}
		}
	}

	std::cout << "PASS\n";
	return true;
}
