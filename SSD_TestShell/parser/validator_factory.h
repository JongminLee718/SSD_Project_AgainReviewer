#pragma once

#include <memory>
#include <string>
#include <stdexcept>
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

		throw std::invalid_argument("INVALID COMMAND");
	}
};
