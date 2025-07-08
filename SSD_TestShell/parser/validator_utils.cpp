#include "validator_utils.h"
#include <stdexcept>

using std::string;

void ValidatorUtils::checkTokenCount(const std::vector<std::string>& tokens, size_t expected)
{
	if (tokens.size() != expected) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}

void ValidatorUtils::validateAddress(const string& address)
{
	int lba;
	try {
		lba = std::stoi(address);
	}
	catch (...) {
		throw std::invalid_argument("INVALID COMMAND");
	}
	if (lba < 0 || lba > 99) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}

void ValidatorUtils::validateDataValue(const string& dataValue)
{
	long long value;
	try {
		value = std::stoll(dataValue, nullptr, 16);
		if (value > MAX_VALID_DATA_VALUE) {
			throw std::invalid_argument("INVALID COMMAND");
		}
	}
	catch (...) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}