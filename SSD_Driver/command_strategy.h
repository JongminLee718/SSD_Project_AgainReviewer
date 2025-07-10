#pragma once
#include <vector>
#include <string>
#include <memory>
#include "buffer_manager.h"

class BufferManager;
struct Command;

class CommandStrategy {
public:
	virtual ~CommandStrategy() = default;
	virtual void execute(BufferManager& bm, std::vector<Command>& commands, Command& new_cmd, std::vector<Command>& commands_to_add, bool& reject_new_cmd) = 0;
};

class WriteStrategy : public CommandStrategy {
public:
	void execute(BufferManager& bm, std::vector<Command>& commands, Command& new_cmd, std::vector<Command>& commands_to_add, bool& reject_new_cmd) override;
};

class EraseStrategy : public CommandStrategy {
public:
	void execute(BufferManager& bm, std::vector<Command>& commands, Command& new_cmd, std::vector<Command>& commands_to_add, bool& reject_new_cmd) override;
};
