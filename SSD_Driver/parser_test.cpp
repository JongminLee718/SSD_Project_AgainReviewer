#include "gmock/gmock.h"
#include "parser.h"
#include <vector>
#include <string>

using namespace testing;
using std::string;
using std::vector;

TEST(ParserTest, ParseReadCommand) {
	Parser parser;
	vector<string> actual = parser.parse("read 0");
	vector<string> expected = { "read", "0" };
	EXPECT_EQ(expected, actual);
}

TEST(ParserTest, ParseWriteCommand) {
	Parser parser;
	vector<string> actual = parser.parse("write 3 0xAAAABBBB");
	vector<string> expected = { "write", "3", "0xAAAABBBB" };
	EXPECT_EQ(expected, actual);
}