#include "parser.h"
#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::istringstream;

vector<string> Parser::parse(const string& input) {
	vector<string> result = split(input);
	if (result[0] == "read") {
		if (result.size() != 2) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		int lba;
		try {
			lba = std::stoi(result[1]);
		}
		catch (...) {
			throw std::invalid_argument("INVALID COMMAND");
		}
		if (lba < 0 || lba > 99) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		return result;
	}
	if (result[0] == "write") {
		if (result.size() != 3) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		int lba;
		try {
			lba = std::stoi(result[1]);
		}
		catch (...) {
			throw std::invalid_argument("INVALID COMMAND");
		}
		if (lba < 0 || lba > 99) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		unsigned int value;
		try {
			value = std::stoul(result[2], nullptr, 16);
		}
		catch (...) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		return result;
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