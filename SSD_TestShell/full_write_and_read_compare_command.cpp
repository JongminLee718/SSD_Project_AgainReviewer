#include "full_write_and_read_compare_command.h"

using std::cout;

void FullWriteAndReadCompareCommand::run(vector<string> commands) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
	std::ostringstream oss;

	for (int i = 0; i < 100; i += 4) {
		uint32_t value = dist(gen);
	
		for (int LBA = i; LBA < i + 4; LBA++) {
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			string randData = oss.str();

			ssd->write(std::to_string(LBA), randData);
			ssd->read(std::to_string(LBA));

			bool result = cc->outputChecker(randData);
			if (!result) {
				std::cout << "FAIL";
				return;
			}
		}
	}

	std::cout << "PASS";
}
