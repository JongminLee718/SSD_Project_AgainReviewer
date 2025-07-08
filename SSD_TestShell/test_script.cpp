#include "shell_command.h"
#include <iomanip>
#include <random>
#include <iostream>
#include <string>
#include <sstream>


class TestScript {
public:
	TestScript(Command* cm, Checker* cc) : cm{ cm }, cc{ cc } {}

	void fullWriteAndReadCompare() {
		bool result;
		for (int i = 0; i < 100; i += 4) {
			static std::mt19937 gen(std::random_device{}());
			std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
			uint32_t value = dist(gen);

			std::ostringstream oss;

			for (int j = 0; j < 4; j++) {
				oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
				string randData = oss.str();

				cm->set("W " + std::to_string(j) + " " + randData);
				result = cm->run();
				if (!result) {
					std::cout << "FAIL";
					return;
				}
				cm->set("R " + std::to_string(j));
				result = cm->run();
				if (!result) {
					std::cout << "FAIL";
					return;
				}

				result = cc->nandOutputChecker(randData);
				if (!result) {
					std::cout << "FAIL";
					return;
				}
			}
		}

		std::cout << "PASS";

	}

	void partialLBAWrite() {
		bool result;
		
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
		uint32_t value = dist(gen);

		std::ostringstream oss;
		
		for (int i = 0; i < 30; i++) {
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			string randData = oss.str();

			cm->set("W " + std::to_string(4) + " " + randData);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}
			cm->set("W " + std::to_string(0) + " " + randData);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}
			cm->set("W " + std::to_string(3) + " " + randData);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}
			cm->set("W " + std::to_string(1) + " " + randData);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}
			cm->set("W " + std::to_string(2) + " " + randData);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}

			for (int j = 0; j < 5; j++) {
				result = cc->nandReadAndChecker(j, randData);
				if (!result) {
					std::cout << "FAIL";
					return;
				}
			}
		}

		std::cout << "PASS";
	}

	void writeReadAging() {
		bool result;

		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<uint32_t>dist(0, 0xFFFFFFFF);
		uint32_t value = dist(gen);

		std::ostringstream oss;

		for (int i = 0; i < 200; i++) {
			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			string randData1 = oss.str();

			cm->set("W " + std::to_string(0) + " " + randData1);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}

			oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
			string randData2 = oss.str();

			cm->set("W " + std::to_string(99) + " " + randData2);
			result = cm->run();
			if (!result) {
				std::cout << "FAIL";
				return;
			}

			result = cc->nandReadAndChecker(99, randData1);

			if (result) {
				std::cout << "FAIL";
				return;
			}
		}

		std::cout << "PASS";
	}

private:
	Command* cm;
	Checker* cc;

};
