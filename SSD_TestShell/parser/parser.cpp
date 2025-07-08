#include "parser.h"
#include "validator_utils.h"
#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::istringstream;

vector<string> Parser::parse(const string& input) {
	vector<string> result = split(input);
	if (result[0] == "read") {
		ValidatorUtils::checkTokenCount(result, 2);
		ValidatorUtils::validateAddress(result[1]);
		return result;
	}
	if (result[0] == "write") {
		ValidatorUtils::checkTokenCount(result, 3);
		ValidatorUtils::validateAddress(result[1]);
		ValidatorUtils::validateDataValue(result[2]);
		return result;
	}
	if (result[0] == "exit" || result[0] == "help" || result[0] == "fullread") {
		ValidatorUtils::checkTokenCount(result, 1);
		return result;
	}
	if (result[0] == "fullwrite") {
		ValidatorUtils::checkTokenCount(result, 2);
		ValidatorUtils::validateDataValue(result[1]);
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
