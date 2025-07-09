#include "gmock/gmock.h"
#include "full_write_and_read_compare_command.h"
#include "partial_lba_write_command.h"
#include "write_read_aging_command.h"
#include "erase_and_write_aging_command.h"

#include <iostream>
#include <sstream>

using namespace testing;

class TestScriptFixture : public Test {
public:
	SsdHandlerMock ssd;
	OutputCheckerMock cc;
	
	std::ostringstream oss;
	std::streambuf* old_buf;

	vector<string> commands;
private:
	void SetUp() override {
		old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection
	}

	void TearDown() override {
		std::cout.rdbuf(old_buf);  // origin buffer restore
	}
};

TEST_F(TestScriptFixture, FullWriteAndReadComapreCommand_Test1) {
	FullWriteAndReadCompareCommand fullWriteAndReadComapre{ &ssd, &cc };
	
	EXPECT_CALL(ssd, write(_, _))
		.Times(100);
	EXPECT_CALL(ssd, read(_))
		.Times(100);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(100)
		.WillRepeatedly(Return(true));
	
	fullWriteAndReadComapre.run(commands);

	EXPECT_EQ(oss.str(), "PASS\n");
}

TEST_F(TestScriptFixture, FullWriteAndReadComapreCommand_Test2) {
	FullWriteAndReadCompareCommand fullWriteAndReadComapre{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(1);
	EXPECT_CALL(ssd, read(_))
		.Times(1);
	EXPECT_CALL(cc, outputChecker(_))
		.WillRepeatedly(Return(false));

	fullWriteAndReadComapre.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, FullWriteAndReadComapreCommand_Test3) {
	FullWriteAndReadCompareCommand fullWriteAndReadComapre{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(2);
	EXPECT_CALL(ssd, read(_))
		.Times(2);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(2)
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	fullWriteAndReadComapre.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, PartialLBAWrite_Test1) {
	PartialLBAWriteCommand partialLBAWrite{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(150);
	EXPECT_CALL(ssd, read(_))
		.Times(150);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(150)
		.WillRepeatedly(Return(true));

	partialLBAWrite.run(commands);

	EXPECT_EQ(oss.str(), "PASS\n");
}

TEST_F(TestScriptFixture, PartialLBAWrite_Test2) {
	PartialLBAWriteCommand partialLBAWrite{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(5);
	EXPECT_CALL(ssd, read(_))
		.Times(1);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(1)
		.WillRepeatedly(Return(false));

	partialLBAWrite.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, PartialLBAWrite_Test3) {
	PartialLBAWriteCommand partialLBAWrite{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(10);
	EXPECT_CALL(ssd, read(_))
		.Times(6);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(6)
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	partialLBAWrite.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, WriteReadAgingCommand_Test1) {
	WriteReadAgingCommand writeReadAging{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(400);
	EXPECT_CALL(ssd, read(_))
		.Times(400);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(400)
		.WillRepeatedly(Return(true));

	writeReadAging.run(commands);

	EXPECT_EQ(oss.str(), "PASS\n");
}

TEST_F(TestScriptFixture, WriteReadAgingCommand_Test2) {
	WriteReadAgingCommand writeReadAging{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(2);
	EXPECT_CALL(ssd, read(_))
		.Times(1);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(1)
		.WillRepeatedly(Return(false));

	writeReadAging.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, WriteReadAgingCommand_Test3) {
	WriteReadAgingCommand writeReadAging{ &ssd, &cc };

	EXPECT_CALL(ssd, write(_, _))
		.Times(2);
	EXPECT_CALL(ssd, read(_))
		.Times(2);
	EXPECT_CALL(cc, outputChecker(_))
		.Times(2)
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	writeReadAging.run(commands);

	EXPECT_EQ(oss.str(), "FAIL\n");
}

TEST_F(TestScriptFixture, EraseAndWriteAging_Test1) {
	EraseAndWriteAgingCommand eraseAndWriteAging{ &ssd };

	EXPECT_CALL(ssd, write(_, _))
		.Times(2940);
	EXPECT_CALL(ssd, erase(_, _))
		.Times(1500);

	eraseAndWriteAging.run(commands);

	EXPECT_EQ(oss.str(), "PASS\n");
}
