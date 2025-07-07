#include "shell_command.h"
#include <iomanip>
#include <random>
#include <iostream>
#include <string>
#include <sstream>

class TestScript {
public:
	TestScript(Command* cm) : cm{ cm } {}

	void fullWriteAndReadCompare() {
		for (int i = 0; i < 100; i += 4) {
			static std::mt19937 gen(std::random_device{}());
			std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
			uint32_t value = dist(gen);

			std::ostringstream oss;
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;

			for (int j = 0; j < 4; j++) {
				cm->set("W " + std::to_string(j) + " " + oss.str());
				cm->run();
				cm->set("R " + std::to_string(j));
				cm->run();
				// TODO
				// ssd_nand.txt와 ssd_output.txt 확인하여 같은지 확인
				// 다르면 FAIL 출력
			}
		}

		std::cout << "PASS" << std::endl;

	}

	void partialLBAWrite() {

		for (int i = 0; i < 120; i++) {
			cm->set("W");
			cm->run();
		}
	}

	void writeReadAging() {

		for (int i = 0; i < 400; i++) {
			cm->set("W");
			cm->run();
		}
	}

private:
	Command* cm;
};
