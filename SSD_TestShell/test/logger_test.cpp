#include "gmock/gmock.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using std::string;
using namespace testing;

class LoggerFixture : public Test {
public:
	Logger& logger = Logger::getInstance();
	const std::string logFile = logger.getLogFile();

	string currentPattern;
	string minuteAfterPattern;
	string currentLogFile;
	string minuteAfterLogFile;
	string currentZipFile;
	string minuteAfterZipFile;

	void createLargeLogFile() {
		std::ofstream out(logFile, std::ios::binary);
		std::vector<char> buffer(10240, 'A');
		out.write(buffer.data(), buffer.size());
		out.flush();
		out.close();
	}

	string generateLogFilename(time_t now, const string& ext) {
		std::tm tm;
		localtime_s(&tm, &now);

		char buffer[64];
		std::snprintf(
			buffer, sizeof(buffer),
			"until_%02d%02d%02d_%02dh_%02dm_%02ds",
			(tm.tm_year % 100),
			tm.tm_mon + 1,
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec
		);

		if (ext == "log") {
			return logDir + std::string(buffer) + ".log";
		}
		else if (ext == "zip") {
			return logDir + std::string(buffer) + ".zip";
		}
		else {
			return logDir + std::string(buffer) + ".unknown";
		}
	}

private:
	string logDir = "logs/";
	std::time_t now;

	void SetUp() override {
		now = std::time(nullptr);
		string currentTimestamp = getTimeStamp(now);
		string minuteAfterTimestamp = getTimeStamp(now + 60);
		currentLogFile = generateLogFilename(now, "log");
		minuteAfterLogFile = generateLogFilename(now + 60, "log");
		currentZipFile = generateLogFilename(now, "zip");
		minuteAfterZipFile = generateLogFilename(now + 60, "zip");

		currentPattern = getLineWithTimeStamp(currentTimestamp);
		minuteAfterPattern = getLineWithTimeStamp(minuteAfterTimestamp);
	}

	void TearDown() override {
		if (std::filesystem::exists(currentLogFile)) {
			std::filesystem::remove(currentLogFile);
		}

		if (std::filesystem::exists(minuteAfterLogFile)) {
			std::filesystem::remove(minuteAfterLogFile);
		}

		if (std::filesystem::exists(currentZipFile)) {
			std::filesystem::remove(currentZipFile);
		}

		if (std::filesystem::exists(minuteAfterZipFile)) {
			std::filesystem::remove(minuteAfterZipFile);
		}

		if (std::filesystem::exists(logFile)) {
			std::filesystem::remove(logFile);
		}
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
	logger.print("some message.", "void __cdecl Foo::bar(void)");

	std::ifstream in(logFile);
	string line, last;
	while (std::getline(in, line))
		if (!line.empty()) last = line;
	in.close();

	EXPECT_TRUE(std::regex_match(last, std::regex(currentPattern)) ||
		std::regex_match(last, std::regex(minuteAfterPattern)));
}

TEST_F(LoggerFixture, PrintMultiLogLine) {
	logger.print("some message.", "void __cdecl Foo::bar(void)");
	logger.print("some message.", "void __cdecl Foo::bar(void)");
	logger.print("some message.", "void __cdecl Foo::bar(void)");

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
}

TEST_F(LoggerFixture, SaveAsLogFile) {
	createLargeLogFile();

	logger.print("some message.", "void __cdecl Foo::bar(void)");

	EXPECT_TRUE(std::filesystem::exists(currentLogFile) ||
		std::filesystem::exists(minuteAfterLogFile));
}

TEST_F(LoggerFixture, SaveAsZipFile) {
	createLargeLogFile();

	logger.print("some message.", "void __cdecl Foo::bar(void)");

	createLargeLogFile();

	logger.print("some message.", "void __cdecl Foo::bar(void)");

	EXPECT_TRUE(std::filesystem::exists(currentZipFile) ||
		std::filesystem::exists(minuteAfterZipFile));
}