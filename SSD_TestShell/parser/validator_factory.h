#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_set>
#include "validator.h"

class ValidatorFactory {
public:
	static std::unique_ptr<Validator> createValidator(const std::string& command) {
		if (command == READ_COMMAND) {
			return std::make_unique<ReadValidator>();
		}
		if (command == WRITE_COMMAND) {
			return std::make_unique<WriteValidator>();
		}
		if (command == FULLWRITE_COMMAND) {
			return std::make_unique<FullWriteValidator>();
		}
		if (ERASE_COMMANDS.contains(command)) {
			return std::make_unique<EraseValidator>();
		}
		if (SIMPLE_COMMANDS.contains(command)) {
			return std::make_unique<SimpleValidator>();
		}
		if (SCRIPT_COMMANDS.contains(command)) {
			return std::make_unique<ScriptValidator>();
		}
		throw std::invalid_argument("INVALID COMMAND");
	}

private:
	static inline const std::string READ_COMMAND = "read";
	static inline const std::string WRITE_COMMAND = "write";
	static inline const std::string FULLWRITE_COMMAND = "fullwrite";
	static inline const std::unordered_set<std::string> ERASE_COMMANDS = { "erase", "erase_range" };
	static inline const std::unordered_set<std::string> SIMPLE_COMMANDS = { "fullread", "exit", "help", "flush" };
	static inline const std::unordered_set<std::string> SCRIPT_COMMANDS =
	{ "1_", "2_", "3_", "4_", "1_FullWriteAndReadCompare",
	"2_PartialLBAWrite", "3_WriteReadAging", "4_EraseAndWriteAging" };
};
