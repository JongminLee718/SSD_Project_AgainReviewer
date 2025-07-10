#include "command_strategy.h"
#include <algorithm> 

void WriteStrategy::execute(BufferManager& bm, std::vector<Command>& commands, Command& new_cmd, std::vector<Command>& commands_to_add, bool& reject_new_cmd) {
	int emptyCnt = 0;
	for (const auto& cmd : commands) {
		if (cmd.isEmpty) emptyCnt++;
	}

	for (int i = 0; i < commands.size(); ++i) {
		if (commands[i].isEmpty || commands[i].type != "E") continue;

		int old_start = commands[i].address;
		int old_end = commands[i].address + commands[i].eraseSize;
		int write_addr = new_cmd.address;

		if (write_addr >= old_start && write_addr < old_end) {
			int required_slots = (write_addr > old_start && write_addr < old_end - 1) ? 2 : 1;
			if (emptyCnt < required_slots) {
				bm.flush();
				bm.addCommandInBuffer(new_cmd.type, new_cmd.address, new_cmd.value);
				reject_new_cmd = true;
				return;
			}

			if (write_addr > old_start) {
				commands_to_add.push_back({ "E", false, old_start, 0, write_addr - old_start });
			}
			if (write_addr < old_end - 1) {
				commands_to_add.push_back({ "E", false, write_addr + 1, 0, old_end - (write_addr + 1) });
			}
			commands[i].isEmpty = true;
		}
	}
}

void EraseStrategy::execute(BufferManager& bm, std::vector<Command>& commands, Command& new_cmd, std::vector<Command>& commands_to_add, bool& reject_new_cmd) {
	for (int i = 0; i < commands.size(); ++i) {
		if (commands[i].isEmpty) continue;

		if (commands[i].type == "E") {
			int old_start = commands[i].address;
			int old_end = commands[i].address + commands[i].eraseSize;
			int new_start = new_cmd.address;
			int new_end = new_cmd.address + new_cmd.eraseSize;

			if (new_start >= old_start && new_end <= old_end) {
				reject_new_cmd = true;
				return;
			}
			if (old_start >= new_start && old_end <= new_end) {
				commands[i].isEmpty = true;
			}
			else if (new_start <= old_end && new_end >= old_start) {
				int merged_start = std::min(old_start, new_start);
				int merged_end = std::max(old_end, new_end);
				int merged_size = merged_end - merged_start;
				if (merged_size <= 10) { // MAX_ERASE_SIZE
					commands[i].isEmpty = true;
					new_cmd.address = merged_start;
					new_cmd.eraseSize = merged_size;
				}
			}
		}
		else if (commands[i].type == "W") {
			int erase_start = new_cmd.address;
			int erase_end = new_cmd.address + new_cmd.eraseSize;
			if (commands[i].address >= erase_start && commands[i].address < erase_end) {
				commands[i].isEmpty = true;
			}
		}
	}
}
