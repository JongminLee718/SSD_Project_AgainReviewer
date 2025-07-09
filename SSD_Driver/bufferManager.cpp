#include "fileio.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;

struct Command {
	std::string type;
	bool isEmpty = true; 
	int address = -1;
	unsigned int value = 0;
	int eraseSize = 0;
};

class BufferManager {
public:
	void initializeBuffer() {
		if (!fs::exists(bufferDir)) {
			fs::create_directory(bufferDir);
		}

		bool found = false;
		for (int i = 1; i <= 5; ++i) {
			for (const auto& entry : fs::directory_iterator(bufferDir)) {
				if (entry.path().filename().string().starts_with(std::to_string(i) + "_")) {
					found = true;
					break;
				}
			}
		}

		for (int i = 1; i <= 5; ++i) {
			if (!found) {
				std::string init_file_name = std::to_string(i) + "_empty";
				std::ofstream(fs::path(bufferDir) / init_file_name);
			}
		}	
	}

	void loadBufferData() {
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
			if (bufferNum < 0 || bufferNum > 5) continue;
		
			int command_addr = std::stoi(parts[2]);
			std::string command_type = parts[1];
			commands[bufferNum].type = command_type;
			commands[bufferNum].address = command_addr;			
			if (command_type == "W") {
				commands[bufferNum].value = std::stoul(parts[3], nullptr, 16);
			}
			else if (command_type == "E") {
				commands[bufferNum].eraseSize = std::stoi(parts[3]);
			}			
			commands[bufferNum].isEmpty = false;
		}			
	}

	void updateBufferFile() {
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
				std::ofstream(fs::path(bufferDir) / file_name);
			}
			std::ofstream(fs::path(bufferDir) / file_name);
		}
	}

	void addCommandInBuffer(const std::string &command_type, int address, int valueOrSize) {
		loadBufferData();

		int emptyCnt = 0;
		for (const auto& cmd : commands) {
			if (cmd.isEmpty) emptyCnt++;
		}

		if (emptyCnt == 0) {
			//flush();
			//initializeBuffer();
		}

		bool cmd_handled = false;
		if (command_type == "E") {
			int new_address = address;
			int new_erasesize = static_cast<int>(valueOrSize);

			for (auto& old_cmd : commands) {
				if (old_cmd.isEmpty || old_cmd.type != "E") continue;
				int old_start = old_cmd.address;
				int old_end = old_cmd.address + old_cmd.eraseSize;
				int new_start = new_address;
				int new_end = new_address + new_erasesize;

				if (new_start >= old_start && new_end <= old_end) {
					cmd_handled = true;
					break;
				}

				if (old_start >= new_start && old_end <= new_end) {
					old_cmd.isEmpty = true;
					continue;
				}

				if (new_start < old_end && new_end > old_start) {
					int merged_start = std::min(old_start, new_start);
					int merged_size = std::max(old_end, new_end) - merged_start;

					if (merged_size <= 10) {
						old_cmd.isEmpty = true;
						new_address = merged_start;
						new_erasesize = merged_size;
					}
				}
			}

			if (cmd_handled) {
				for (auto& entry : fs::directory_iterator(bufferDir)) {
					fs::remove(entry.path());
				}

				updateBufferFile();
				return;
			}
			address = new_address;
			valueOrSize = new_erasesize;
		}

		for (auto& cmd : commands) {
			//�ּҰ� ���� 
			if (cmd.address == address && !cmd.isEmpty) {
				cmd.isEmpty = true;
			}
		}

		for (auto& cmd : commands) {
			if (cmd.isEmpty) {
				cmd.isEmpty = false;
				cmd.address = address;
				cmd.type = command_type;
				if (cmd.type == "W") {
					cmd.value = valueOrSize;
				}
				else if (cmd.type == "E") {
					cmd.eraseSize = (int)valueOrSize;
				}
				break;
			}
		}

		for (auto& entry : fs::directory_iterator(bufferDir)) {
			fs::remove(entry.path());
		}

		updateBufferFile();
	}

	bool readFromBuffer(int address, unsigned int& outValue) {
		loadBufferData();

		for (int i = 1; i <= 5; i++) {
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

private:
	const std::string bufferDir = "buffer";
	std::vector<Command> commands;
};