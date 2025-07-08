#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_set>
#include "validator.h"

class ValidatorFactory {
public:
	static std::unique_ptr<Validator> createValidator(const std::string& command) {
		if (command == "read") {
			return std::make_unique<ReadValidator>();
		}
		if (command == "write") {
			return std::make_unique<WriteValidator>();
		}
		if (command == "fullwrite") {
			return std::make_unique<FullWriteValidator>();
		}
		if (command == "fullread" || command == "exit" || command == "help") {
			return std::make_unique<SimpleValidator>();
		}
		if (startsWith(command, "1_") || startsWith(command, "2_") || startsWith(command, "3_")) {
			return std::make_unique<ScriptValidator>();
		}

		throw std::invalid_argument("INVALID COMMAND");
	}

private:
	static bool startsWith(const std::string& str, const std::string& prefix) {
		return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
	}

	const std::unordered_set<std::string> simpleCommands = {
	"fullread", "exit", "help" };

	const std::unordered_set<std::string> scriptCommands = {
		"1_FullWriteAndReadCompare", "2_PartialLBAWrite", "3_WriteReadAging" };


};
