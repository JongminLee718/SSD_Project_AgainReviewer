#include "gmock/gmock.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

using std::string;
using namespace testing;

TEST(LoggerTest, PrintLogLine) {
	Logger logger;
	const std::string logFile = logger.getLogFile();

	logger.print("Foo.bar()", "some message.");

	std::ifstream in(logFile);
	ASSERT_TRUE(in.is_open());

	std::string line, last;
	while (std::getline(in, line))
		if (!line.empty()) last = line;
	in.close();

	std::time_t now = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &now);
	char buf[20];
	std::strftime(buf, sizeof(buf), "%y.%m.%d %H:%M", &tm);
	std::string timestamp(buf);

	std::string pattern_now =
		R"(\[)" + timestamp + R"(\] Foo\.bar\(\)\s*: some message\.)";
	std::string pattern_prev =
		R"(\[)" + timestamp + R"(\] Foo\.bar\(\)\s*: some message\.)";

	EXPECT_TRUE(std::regex_match(last, std::regex(pattern_now)) ||
		std::regex_match(last, std::regex(pattern_prev)));

	std::filesystem::remove(logFile);
}