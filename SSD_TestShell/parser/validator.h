#include <string>
#include <vector>
#include "validator_utils.h"

class Validator {
public:
	Validator() = default;
	~Validator() = default;

	virtual void validate(const std::vector<std::string> token) = 0;
};

class ReadValidator : public Validator {
public:
	void validate(const std::vector<std::string> tokens) override {
		ValidatorUtils::checkTokenCount(tokens, EXPECTED_NUM_TOKENS);
		ValidatorUtils::checkInteger(tokens[1]);
	}
private:
	const size_t EXPECTED_NUM_TOKENS = 2;
};

class WriteValidator : public Validator {
public:
	void validate(const std::vector<std::string> tokens) override {
		ValidatorUtils::checkTokenCount(tokens, EXPECTED_NUM_TOKENS);
		ValidatorUtils::checkInteger(tokens[1]);
		ValidatorUtils::checkHexValue(tokens[2]);
	}
private:
	const size_t EXPECTED_NUM_TOKENS = 3;
};

class FullWriteValidator : public Validator {
public:
	void validate(const std::vector<std::string> tokens) override {
		ValidatorUtils::checkTokenCount(tokens, EXPECTED_NUM_TOKENS);
		ValidatorUtils::checkHexValue(tokens[1]);
	}
private:
	const size_t EXPECTED_NUM_TOKENS = 2;
};

class SimpleValidator : public Validator {
public:
	void validate(const std::vector<std::string> tokens) override {
		ValidatorUtils::checkTokenCount(tokens, EXPECTED_NUM_TOKENS);
	}
private:
	const size_t EXPECTED_NUM_TOKENS = 1;
};

class ScriptValidator : public Validator {
public:
	void validate(const std::vector<std::string> tokens) override {
		ValidatorUtils::checkTokenCount(tokens, EXPECTED_NUM_TOKENS);
	}
private:
	const size_t EXPECTED_NUM_TOKENS = 1;
};