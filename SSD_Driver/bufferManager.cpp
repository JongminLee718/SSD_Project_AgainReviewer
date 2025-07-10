#include "bufferManager.h"
#include "fileio.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>

namespace fs = std::filesystem;

BufferManager::BufferManager(std::string nandPath) : nandPath(nandPath) {}

void BufferManager::initializeBuffer() {
	if (!fs::exists(bufferDir)) {
		fs::create_directory(bufferDir);
	}

	std::vector<bool> found_numbers(6, false);
	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		std::string filename = entry.path().filename().string();
		if (!filename.empty() && isdigit(filename[0])) {
			int num = filename[0] - '0';
			if (num >= 1 && num <= 5) {
				found_numbers[num] = true;
			}
		}
	}

	for (int i = 1; i <= 5; ++i) {
		if (!found_numbers[i]) {
			std::string init_file_name = std::to_string(i) + "_empty";
			std::ofstream(fs::path(bufferDir) / init_file_name);
		}
	}
}

void BufferManager::loadBufferData() {
	commands.clear();
	commands.resize(5);

	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		std::string file_name = entry.path().filename().string();
		std::stringstream ss(file_name);
		std::string segment;
		std::vector<std::string> parts;

		while (std::getline(ss, segment, '_')) {
			parts.push_back(segment);
		}

		if (parts.size() < 2) continue;
		if (parts[1] == "empty") continue;

		int bufferNum = std::stoi(parts[0]);
		if (bufferNum < 1 || bufferNum > 5) continue;

		int command_addr = std::stoi(parts[2]);
		std::string command_type = parts[1];
		commands[bufferNum - 1].type = command_type;
		commands[bufferNum - 1].address = command_addr;
		if (command_type == "W") {
			commands[bufferNum - 1].value = std::stoul(parts[3], nullptr, 16);
		}
		else if (command_type == "E") {
			commands[bufferNum - 1].eraseSize = std::stoi(parts[3]);
		}
		commands[bufferNum - 1].isEmpty = false;
	}
}

void BufferManager::updateBufferFile() {
	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		fs::remove(entry.path());
	}

	for (int i = 0; i < 5; i++) {
		std::string file_name = std::to_string(i + 1) + '_';
		if (commands[i].isEmpty) {
			file_name += "empty";
		}
		else {
			file_name += commands[i].type + "_";
			file_name += std::to_string(commands[i].address) + "_";
			if (commands[i].type == "W") {
				std::stringstream ss;
				ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << commands[i].value;
				file_name += ss.str();
			}
			else if (commands[i].type == "E") {
				file_name += std::to_string(commands[i].eraseSize);
			}
		}
		std::ofstream(fs::path(bufferDir) / file_name);
	}
}

void BufferManager::addCommandInBuffer(const std::string& command_type, int address, int valueOrSize) {
	loadBufferData();

	int emptyCnt = 0;
	for (const auto& cmd : commands) {
		if (cmd.isEmpty) emptyCnt++;
	}
	if (emptyCnt == 0) {
		flush();
		loadBufferData();
		emptyCnt = 5;
	}

	Command new_cmd;
	new_cmd.isEmpty = false;
	new_cmd.type = command_type;
	new_cmd.address = address;
	if (command_type == "W") new_cmd.value = valueOrSize;
	else new_cmd.eraseSize = valueOrSize;

	bool reject_new_cmd = false;

	std::vector<Command> commands_to_add;

	for (int i = 0; i < commands.size(); ++i) {
		if (commands[i].isEmpty) continue;

		if (new_cmd.type == "W" && commands[i].type == "E") {
			int old_start = commands[i].address;
			int old_end = commands[i].address + commands[i].eraseSize;
			int write_addr = new_cmd.address;

			if (write_addr >= old_start && write_addr < old_end) {
				int required_slots = (write_addr > old_start && write_addr < old_end - 1) ? 2 : 1;

				if (emptyCnt < required_slots) {
					flush();
					addCommandInBuffer(command_type, address, valueOrSize);
					return;
				}
				if (write_addr > old_start) {
					Command prefix_erase;
					prefix_erase.isEmpty = false;
					prefix_erase.type = "E";
					prefix_erase.address = old_start;
					prefix_erase.eraseSize = write_addr - old_start;
					commands_to_add.push_back(prefix_erase);
				}
				if (write_addr < old_end - 1) {
					Command suffix_erase;
					suffix_erase.isEmpty = false;
					suffix_erase.type = "E";
					suffix_erase.address = write_addr + 1;
					suffix_erase.eraseSize = old_end - (write_addr + 1);
					commands_to_add.push_back(suffix_erase);
				}
				commands[i].isEmpty = true;
			}
		}
		else if (new_cmd.type == "E" && commands[i].type == "E") {
			int old_start = commands[i].address;
			int old_end = commands[i].address + commands[i].eraseSize;
			int new_start = new_cmd.address;
			int new_end = new_cmd.address + new_cmd.eraseSize;

			if (new_start >= old_start && new_end <= old_end) {
				reject_new_cmd = true;
				break;
			}
			else if (old_start >= new_start && old_end <= new_end) {
				commands[i].isEmpty = true;
			}
			else if (new_start <= old_end && new_end >= old_start) {
				int merged_start = std::min(old_start, new_start);
				int merged_end = std::max(old_end, new_end);
				int merged_size = merged_end - merged_start;
				commands[i].isEmpty = true;
				new_cmd.address = merged_start;
				new_cmd.eraseSize = merged_size;
			}
		}
		else if (new_cmd.type == "E" && commands[i].type == "W") {
			int erase_start = new_cmd.address;
			int erase_end = new_cmd.address + new_cmd.eraseSize;
			int write_addr = commands[i].address;

			if (write_addr >= erase_start && write_addr < erase_end) {
				commands[i].isEmpty = true;
			}
		}
	}

	if (!reject_new_cmd) {
		commands_to_add.push_back(new_cmd);
	}

	for (const auto& cmd_to_add : commands_to_add) {
		bool replaced = false;
		for (auto& old_cmd : commands) {
			if (!old_cmd.isEmpty && old_cmd.address == cmd_to_add.address) {
				old_cmd = cmd_to_add;
				replaced = true;
				break;
			}
		}

		if (!replaced) {
			for (auto& slot : commands) {
				if (slot.isEmpty) {
					slot = cmd_to_add;
					break;
				}
			}
		}
	}

	updateBufferFile();
}

bool BufferManager::readFromBuffer(int address, unsigned int& outValue) {
	loadBufferData();

	for (int i = 4; i >= 0; i--) {
		const auto& cmd = commands[i];
		if (cmd.isEmpty) continue;

		if (cmd.type == "E" && cmd.address <= address && address < (cmd.address + cmd.eraseSize)) {
			outValue = 0x00000000;
			return true;
		}
		else if (cmd.type == "W" && cmd.address == address) {
			outValue = cmd.value;
			return true;
		}
	}
	return false;
}

void BufferManager::flush() {
	loadBufferData();

	FileInOut file_io(nandPath);
	std::vector<unsigned int> ssd_memory = file_io.nandData;

	for (const auto& cmd : commands) {
		if (cmd.isEmpty) continue;
		if (cmd.type == "W") {
			if (cmd.address >= 0 && cmd.address < ssd_memory.size()) {
				ssd_memory[cmd.address] = cmd.value;
			}
		}
		else if (cmd.type == "E") {
			for (int i = 0; i < cmd.eraseSize; i++) {
				int current_address = cmd.address + i;
				if (current_address >= 0 && current_address < ssd_memory.size()) {
					ssd_memory[current_address] = 0x00000000;
				}
			}
		}
	}

	std::ofstream nandFile(nandPath);
	for (const auto& data : ssd_memory) {
		nandFile << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data << "\n";
	}

	for (auto& cmd : commands) {
		cmd.isEmpty = true;
	}

	updateBufferFile();
}
