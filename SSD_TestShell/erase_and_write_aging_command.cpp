#include "erase_and_write_aging_command.h"

using std::cout;

void EraseAndWriteAgingCommand::run(vector<string> commands) {
	static std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
	std::ostringstream oss;

	for (int i = 0; i < 30; i++) {
		ssd->erase(std::to_string(0), std::to_string(3));
		for (int LBA = 2; LBA < 100; LBA+=2) {
			uint32_t value = dist(gen);
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			string randData = oss.str();
			ssd->write(std::to_string(LBA), randData);
			
			value = dist(gen);
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			randData = oss.str();
			ssd->write(std::to_string(LBA), randData);
			
			if(LBA == 98)
				ssd->erase(std::to_string(LBA), std::to_string(2));
			else 
				ssd->erase(std::to_string(LBA), std::to_string(3));
		}
	}

	std::cout << "PASS";

}