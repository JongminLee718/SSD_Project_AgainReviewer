#include "validator_utils.h"
#include <stdexcept>

using std::string;

void ValidatorUtils::checkTokenCount(const std::vector<std::string>& tokens, size_t expected)
{
	if (tokens.size() != expected) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}

void ValidatorUtils::checkInteger(const std::string& address) {
	try {
		std::stoi(address);
	}
	catch (...) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}

void ValidatorUtils::checkHexValue(const std::string& dataValue) {
	if (dataValue.size() < 3 || !(dataValue[0] == '0' && (dataValue[1] == 'x' || dataValue[1] == 'X'))) {
		throw std::invalid_argument("INVALID COMMAND");
	}

	const std::string hexPart = dataValue.substr(2);
	if (hexPart.empty()) {
		throw std::invalid_argument("INVALID COMMAND");
	}

	for (char c : hexPart) {
		if (!std::isxdigit(static_cast<unsigned char>(c))) {
			throw std::invalid_argument("INVALID COMMAND");
		}
	}
}