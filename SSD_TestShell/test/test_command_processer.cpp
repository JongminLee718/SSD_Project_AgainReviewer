#include "gmock/gmock.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "../command_processer.h"

using namespace testing;
using std::string;
using std::vector;
using std::to_string;

class CommandProcesserFixture : public Test {
public:
	CommandProcesser commandProcesser;
	SsdHandlerMock mockSssHandler;

	std::ostringstream oss;
	std::streambuf* old_buf;

	string READ = "read";
	string WRITE = "write";
	string FULL_READ = "fullread";
	string FULL_WRITE = "fullwrite";

	string LBA = "0";
	string FAIL_LBA = "100";
	string DEFAULT_DATA ="0x00000000";
	string ERROR_PATTERN = "ERROR";

	int MAX_LBA = 100;

	string getReadFormat(string lba, string expect) {
		return "[Read] LBA " + lba + " : "+ expect +"\n";
	}

	string getWriteFormat() {
		return "[Write] Done\n";
	}

	string getFullReadFormat() {
		string result;
		for (int lba = 0; lba < MAX_LBA; lba++) {
			result = result + getReadFormat(to_string(lba), DEFAULT_DATA);
		}
		return result;
	}

	string getFullWriteFormat() {
		return "[Write] Done\n";
	}

private:
	void SetUp() override {
		commandProcesser.setSsdInterface(&mockSssHandler);
		old_buf = std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(old_buf);
	}
};

TEST_F(CommandProcesserFixture, ReadCommand_Success) {
	vector<string> commands = { READ , LBA };
	string actual = getReadFormat(LBA,DEFAULT_DATA);

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(DEFAULT_DATA));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, ReadCommand_Fail) {
	vector<string> commands = { READ , LBA };
	string actual = "[Read] ERROR\n";
	

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Success) {
	vector<string> commands = { WRITE , LBA, DEFAULT_DATA };
	string actual = getWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_,_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(""));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Fail) {
	vector<string> commands = { WRITE , FAIL_LBA, DEFAULT_DATA };
	string actual = "[Write] ERROR\n";

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(1);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullReadCommand_Success) {
	vector<string> commands = { FULL_READ };
	string actual = getFullReadFormat();

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(100);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillRepeatedly(Return(DEFAULT_DATA));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullWriteCommand_Success) {
	vector<string> commands = { FULL_WRITE , DEFAULT_DATA};
	string actual = getFullWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(100);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillRepeatedly(Return(""));

	commandProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}