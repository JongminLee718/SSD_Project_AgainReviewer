#include "parser.h"
#include "validator_factory.h"
#include "logger.h"
#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::istringstream;

vector<string> Parser::parse(const string& input) {
	vector<string> tokens = split(input);

	try {
		if (tokens.empty()) {
			throw std::invalid_argument("INVALID COMMAND");
		}

		auto validator = ValidatorFactory::createValidator(tokens[0]);
		validator->validate(tokens);
	}
	catch (const std::invalid_argument& e) {
		LOG("EXCEPTION: " + std::string(e.what()));
		throw;
	}

	LOG("SUCCESS: PARSED " + tokens[0] + " COMMAND");
	return tokens;
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
