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

TEST_F(ParserFixture, ReadSuccess2) {
	vector<string> actual = parser.parse("read 255");
	vector<string> expected = { "read", "255" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, ReadInvalidLength) {
	EXPECT_THROW(parser.parse("read 0 0xAAAABBBB"), std::invalid_argument);
}

TEST_F(ParserFixture, ReadInvalidArgument) {
	EXPECT_THROW(parser.parse("read foo"), std::invalid_argument);
}

TEST_F(ParserFixture, WriteSuccess) {
	vector<string> actual = parser.parse("write 3 0x100000000");
	vector<string> expected = { "write", "3", "0x100000000" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, WriteSuccess2) {
	vector<string> actual = parser.parse("write 3 0X100000000");
	vector<string> expected = { "write", "3", "0X100000000" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, WriteInvalidLength) {
	EXPECT_THROW(parser.parse("write 3"), std::invalid_argument);
}

TEST_F(ParserFixture, WriteInvalidArgument) {
	EXPECT_THROW(parser.parse("write 0 bar"), std::invalid_argument);
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

TEST_F(ParserFixture, ScriptLongSuccess) {
	vector<string> actual = parser.parse("1_FullWriteAndReadCompare");
	vector<string> expected = { "1_FullWriteAndReadCompare" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, ScriptShortSuccess) {
	vector<string> actual = parser.parse("1_");
	vector<string> expected = { "1_" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, EraseSuccess) {
	vector<string> actual = parser.parse("erase 1 30");
	vector<string> expected = { "erase", "1", "30" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, EraseInvalidArgument) {
	EXPECT_THROW(parser.parse("erase foo 30"), std::invalid_argument);
}

TEST_F(ParserFixture, FlushSuccess) {
	vector<string> actual = parser.parse("flush");
	vector<string> expected = { "flush" };
	EXPECT_EQ(expected, actual);
}

TEST_F(ParserFixture, UnknownCommand) {
	EXPECT_THROW(parser.parse("unknown, 0"), std::invalid_argument);
}