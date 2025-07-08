#include "parser.h"
#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::istringstream;

vector<string> Parser::parse(const string& input) {
	vector<string> result = split(input);
	if (result[0] == "read") {
		checkTokenCount(result, 2);
		validateAddress(result[1]);
		return result;
	}
	if (result[0] == "write") {
		checkTokenCount(result, 3);
		validateAddress(result[1]);
		validateDataValue(result[2]);
		return result;
	}
	if (result[0] == "exit" || result[0] == "help") {
		checkTokenCount(result, 1);
	}
	throw std::invalid_argument("INVALID COMMAND");
}

vector<string> Parser::split(const string& input) {
	istringstream inputStream(input);
	vector<std::string> tokens;
	string token;
	while (inputStream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

void Parser::checkTokenCount(std::vector<std::string>& result, int expectedCount)
{
	if (result.size() != expectedCount) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}

void Parser::validateAddress(const string& address)
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

void Parser::validateDataValue(const string& dataValue)
{
	unsigned int value;
	try {
		value = std::stoul(dataValue, nullptr, 16);
	}
	catch (...) {
		throw std::invalid_argument("INVALID COMMAND");
	}
}
