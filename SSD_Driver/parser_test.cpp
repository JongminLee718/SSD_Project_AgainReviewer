#include "gmock/gmock.h"

TEST(ParserTest, ParseReadCommand){
	Parser parser;
	std::vector actual = parser.parse("Read 0");
	EXPECT_EQ(["Read", "0"], actual);
}

TEST(ParserTest, ParseWriteCommand){
	Parser parser;
	std::vector actual = parser.parse("write 3 0xAAAABBBB");
	EXPECT_EQ(["write", "3", "0xAAAABBBB"], actual);
}