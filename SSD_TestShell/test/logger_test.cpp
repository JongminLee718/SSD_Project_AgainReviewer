#include "gmock/gmock.h"
#include <filesystem>
#include <fstream>
#include <string>

using std::string;
using namespace testing;

TEST(LoggerTest, CreateLogFile) {
	Logger.logger;
	const std::string logFile = logger.getLogFile();

	if (std::filesystem::exists(logFile)) {
		std::filesystem::remove(logFile);
	}

	logger.createLogFile();

	EXPECT_TRUE(std::filesystem::exists(logFile));

	std::filesystem::remove(logFile);
}

TEST(LoggerTest, PrintLogLine) {
	Logger logger;
	const std::string logFile = logger.getLogFile();

	logger.createLogFile();

	logger.print("Foo.bar()", "some message.");

	std::ifstream in(logFile);
	ASSERT_TRUE(in.is_open());

	std::string line, last;
	while (std::getline(in, line))
		if (!line.empty()) last = line;
	in.close();

	const std::string pattern =
		R"(\[\d{2}\.\d{2}\.\d{2} \d{2}:\d{2}\] Foo\.bar\(\)\s*: some message\.)";

	EXPECT_THAT(last, MatchesRegex(pattern));

	std::filesystem::remove(logFile);
}