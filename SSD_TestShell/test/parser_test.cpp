#include "gmock/gmock.h"
#include "parser.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace testing;
using std::string;
using std::vector;

class ParserFixture : public Test {
public:
	Parser parser;
};

TEST_F(ParserFixture, ReadSuccess) {
	vector<string> actual = parser.parse("read 0");
	vector<string> expected = { "read", "0" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, ReadInvalidLength) {
	EXPECT_THROW(parser.parse("read 0 0xAAAABBBB"), std::invalid_argument);
}

TEST_F(ParserFixture, ReadInvalidArgument) {
	EXPECT_THROW(parser.parse("read 255"), std::invalid_argument);
}

TEST_F(ParserFixture, WriteSuccess) {
	vector<string> actual = parser.parse("write 3 0xAAAABBBB");
	vector<string> expected = { "write", "3", "0xAAAABBBB" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, WriteInvalidLength) {
	EXPECT_THROW(parser.parse("write 3"), std::invalid_argument);
}

TEST_F(ParserFixture, WriteInvalidArgument) {
	EXPECT_THROW(parser.parse("write 3 0xAAAAAAAAA"), std::invalid_argument);
}

TEST_F(ParserFixture, ExitSuccess) {
	vector<string> actual = parser.parse("exit");
	vector<string> expected = { "exit" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, HelpSuccess) {
	vector<string> actual = parser.parse("help");
	vector<string> expected = { "help" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, FullwriteSuccess) {
	vector<string> actual = parser.parse("fullwrite 0xAAAABBBB");
	vector<string> expected = { "fullwrite", "0xAAAABBBB" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, FullreadSuccess) {
	vector<string> actual = parser.parse("fullread");
	vector<string> expected = { "fullread" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, UnknownCommand) {
	EXPECT_THROW(parser.parse("unknown, 0"), std::invalid_argument);
}