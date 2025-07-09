#include "write_read_aging_command.h"

using std::cout;

void WriteReadAgingCommand::run(vector<string> commands) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
	std::ostringstream oss;

	for (int i = 0; i < 200; i++) {
		uint32_t value = dist(gen);
		oss.str("");
		oss.clear();
		oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
		string randData = oss.str();

		ssd->write(std::to_string(0), randData);
		ssd->write(std::to_string(99), randData);

		ssd->read(std::to_string(0));
		bool result = cc->outputChecker(randData);
		if (!result) {
			std::cout << "FAIL\n";
			return;
		}

		ssd->read(std::to_string(99));
		result = cc->outputChecker(randData);
		if (!result) {
			std::cout << "FAIL\n";
			return;
		}
	}

	std::cout << "PASS\n";
}
