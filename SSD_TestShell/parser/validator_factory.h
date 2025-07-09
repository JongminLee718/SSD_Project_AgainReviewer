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
		if (command == "1_" || command == "2_" || command == "3_" ||
			command == "1_FullWriteAndReadCompare" || command == "2_PartialLBAWrite" || command == "3_WriteReadAging") {
			return std::make_unique<ScriptValidator>();
		}

		throw std::invalid_argument("INVALID COMMAND");
	}
};
