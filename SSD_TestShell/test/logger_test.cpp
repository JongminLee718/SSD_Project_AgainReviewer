#include "gmock/gmock.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

using std::string;
using namespace testing;

class LoggerFixture : public Test {
public:
	Logger logger;
	const std::string logFile = logger.getLogFile();

	string currentPattern;
	string minuteAfterPattern;

private:
	void SetUp() override {
		std::time_t now = std::time(nullptr);
		string currentTimestamp = getTimeStamp(now);
		string minuteAfterTimestamp = getTimeStamp(now - 60);

		currentPattern = getLineWithTimeStamp(currentTimestamp);
		minuteAfterPattern = getLineWithTimeStamp(minuteAfterTimestamp);
	}

	string getTimeStamp(std::time_t now) {
		std::tm tm;
		localtime_s(&tm, &now);
		char buf[20];
		std::strftime(buf, sizeof(buf), "%y.%m.%d %H:%M", &tm);
		return string(buf);
	}

	string getLineWithTimeStamp(const string& timestamp) {
		return R"(\[)" + timestamp + R"(\] Foo\.bar\(\)\s*: some message\.)";
	}
};

TEST_F(LoggerFixture, PrintLogLine) {
	logger.print("Foo.bar()", "some message.");

	std::ifstream in(logFile);
	string line, last;
	while (std::getline(in, line))
		if (!line.empty()) last = line;
	in.close();

	EXPECT_TRUE(std::regex_match(last, std::regex(currentPattern)) ||
		std::regex_match(last, std::regex(minuteAfterPattern)));

	std::filesystem::remove(logFile);
}

TEST_F(LoggerFixture, PrintMultiLogLine) {
	logger.print("Foo.bar()", "some message.");
	logger.print("Foo.bar()", "some message.");
	logger.print("Foo.bar()", "some message.");

	std::ifstream in(logFile);
	std::vector<std::string> lines;
	string line;
	while (std::getline(in, line)) {
		if (!line.empty())
			lines.push_back(line);
	}
	in.close();

	string line1 = lines[lines.size() - 3];
	string line2 = lines[lines.size() - 2];
	string line3 = lines[lines.size() - 1];

	EXPECT_TRUE(std::regex_match(line1, std::regex(currentPattern)) ||
		std::regex_match(line1, std::regex(minuteAfterPattern)));

	EXPECT_TRUE(std::regex_match(line2, std::regex(currentPattern)) ||
		std::regex_match(line2, std::regex(minuteAfterPattern)));

	EXPECT_TRUE(std::regex_match(line3, std::regex(currentPattern)) ||
		std::regex_match(line3, std::regex(minuteAfterPattern)));

	std::filesystem::remove(logFile);
}