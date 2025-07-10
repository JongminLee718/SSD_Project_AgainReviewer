#include "buffer_manager.h"
#include "command_strategy.h"
#include "fileio.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include "main.h"
#include "ssd.h"
#include <map>

namespace fs = std::filesystem;

BufferManager::BufferManager(std::string nandPath) : nandPath(nandPath) {}

void BufferManager::initializeBuffer() {
	if (!fs::exists(bufferDir)) {
		fs::create_directory(bufferDir);
	}
	makeBufferFiles();
}

void BufferManager::makeBufferFiles()
{
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
		parsingCommandBuffer(parts);
	}
}

void BufferManager::parsingCommandBuffer(const std::vector<std::string> parts) {
	if (parts.size() < 2) return;
	if (parts[1] == "empty") return;

	int bufferNum = std::stoi(parts[0]);
	if (bufferNum <= 0 || bufferNum > 5) return;

	std::string command_type = parts[1];
	int command_addr = std::stoi(parts[2]);
	int erase_size = std::stoi(parts[3]);
	int value = std::stoul(parts[3], nullptr, 16);

	commands[bufferNum - 1].type = command_type;
	commands[bufferNum - 1].address = command_addr;
	if (command_type == "W") {
		commands[bufferNum - 1].value = value;
	}
	else if (command_type == "E") {
		commands[bufferNum - 1].eraseSize = erase_size;
	}
	commands[bufferNum - 1].isEmpty = false;
}

void BufferManager::updateBufferFile() {
	std::vector<fs::path> current_paths(5 + 1);
	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		std::string filename = entry.path().filename().string();
		if (!filename.empty() && isdigit(filename[0])) {
			int num = filename[0] - '0';
			if (num >= 1 && num <= 5) {
				current_paths[num] = entry.path();
			}
		}
	}

	for (int i = 0; i < 5; ++i) {
		std::string desired_filename_str;
		generateFileFormatName(i, desired_filename_str);
		renameBuffers(desired_filename_str, current_paths, i);
	}
}

void BufferManager::renameBuffers(std::string& desired_filename_str, std::vector<std::filesystem::path>& current_paths, int i)
{
	fs::path desired_path = fs::path(bufferDir) / desired_filename_str;
	fs::path current_path = current_paths[i + 1];
	fs::rename(current_path, desired_path);
}

void BufferManager::generateFileFormatName(int i, std::string& desired_filename_str)
{
	std::string file_prefix = std::to_string(i + 1) + '_';
	if (commands[i].isEmpty) {
		desired_filename_str = file_prefix + "empty";
	}
	else {
		desired_filename_str = file_prefix + commands[i].type + "_";
		desired_filename_str += std::to_string(commands[i].address) + "_";
		if (commands[i].type == "W") {
			std::stringstream ss;
			ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << commands[i].value;
			desired_filename_str += ss.str();
		}
		else {
			desired_filename_str += std::to_string(commands[i].eraseSize);
		}
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
	}

	std::map<std::string, std::unique_ptr<CommandStrategy>> strategies;
	strategies["W"] = std::make_unique<WriteStrategy>();
	strategies["E"] = std::make_unique<EraseStrategy>();

	auto it = strategies.find(command_type);
	if (it == strategies.end()) {
		return;
	}

	Command new_cmd = { command_type, false, address, (unsigned int)valueOrSize, valueOrSize };
	if (command_type == "W") {
		new_cmd.eraseSize = 0;
	}
	else {
		new_cmd.value = 0;
	}

	bool reject_new_cmd = false;
	std::vector<Command> commands_to_add;

	it->second->execute(*this, commands, new_cmd, commands_to_add, reject_new_cmd);

	if (reject_new_cmd) {
		updateBufferFile();
		return;
	} 
	else {
		commands_to_add.push_back(new_cmd);
	}

	updateBufferWithNewCommands(commands_to_add);
	updateBufferFile();
}

void BufferManager::updateBufferWithNewCommands(const std::vector<Command>& commands_to_add) {
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
			FileInOut fileio(NAND_FILE_PATH);
			SSD ssd(fileio.nandData);
			std::string result;

			if (cmd.address >= 0 && cmd.address < ssd_memory.size()) {
				result = ssd.doWriteCmd(cmd.address, cmd.value);
			}
			ssd.storeNand();
		}
		else if (cmd.type == "E") {
			FileInOut fileio(NAND_FILE_PATH);
			SSD ssd(fileio.nandData);
			std::string result;
			result = ssd.doEraseCmd(cmd.address, cmd.eraseSize);
			ssd.storeNand();
		}
	}

	for (auto& cmd : commands) {
		cmd.isEmpty = true;
	}

	updateBufferFile();
}


