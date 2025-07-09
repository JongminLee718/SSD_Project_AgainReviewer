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

TEST(LoggerTest, PrintMultiLogLine) {
	Logger logger;
	const std::string logFile = logger.getLogFile();

	logger.print("Foo.bar()", "some message.");
	logger.print("Foo.baz()", "some new message.");
	logger.print("Foo.qux()", "some newest message.");

	std::ifstream in(logFile);
	ASSERT_TRUE(in.is_open());

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(in, line)) {
		if (!line.empty())
			lines.push_back(line);
	}
	in.close();

	std::string line1 = lines[lines.size() - 3];
	std::string line2 = lines[lines.size() - 2];
	std::string line3 = lines[lines.size() - 1];

	std::time_t now = std::time(nullptr);

	std::tm tm_now;
	localtime_s(&tm_now, &now);
	char buf_now[20];
	std::strftime(buf_now, sizeof(buf_now), "%y.%m.%d %H:%M", &tm_now);
	std::string ts_now(buf_now);

	std::tm tm_prev;
	now -= 60;
	localtime_s(&tm_prev, &now);
	char buf_prev[20];
	std::strftime(buf_prev, sizeof(buf_prev), "%y.%m.%d %H:%M", &tm_prev);
	std::string ts_prev(buf_prev);

	std::string pattern1_now = R"(\[)" + ts_now + R"(\] Foo\.bar\(\)\s*: some message\.)";
	std::string pattern1_prev = R"(\[)" + ts_prev + R"(\] Foo\.bar\(\)\s*: some message\.)";

	std::string pattern2_now = R"(\[)" + ts_now + R"(\] Foo\.baz\(\)\s*: some new message\.)";
	std::string pattern2_prev = R"(\[)" + ts_prev + R"(\] Foo\.baz\(\)\s*: some new message\.)";

	std::string pattern3_now = R"(\[)" + ts_now + R"(\] Foo\.qux\(\)\s*: some newest message\.)";
	std::string pattern3_prev = R"(\[)" + ts_prev + R"(\] Foo\.qux\(\)\s*: some newest message\.)";

	EXPECT_TRUE(std::regex_match(line1, std::regex(pattern1_now)) ||
		std::regex_match(line1, std::regex(pattern1_prev)));

	EXPECT_TRUE(std::regex_match(line2, std::regex(pattern2_now)) ||
		std::regex_match(line2, std::regex(pattern2_prev)));

	EXPECT_TRUE(std::regex_match(line3, std::regex(pattern3_now)) ||
		std::regex_match(line3, std::regex(pattern3_prev)));

	std::filesystem::remove(logFile);
}