#include "gmock/gmock.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "../checker.h"
#include "../command_processor.h"

using namespace testing;
using std::string;
using std::vector;
using std::to_string;

class CommandProcesserFixture : public Test {
public:
	OutputChecker checker;
	SsdHandlerMock mockSssHandler;

	CommandProcessor mockCmdProcesser{ &mockSssHandler, &checker };

	SsdHandler ssdHandler;
	CommandProcessor realCmdProcesser{ &ssdHandler, &checker };


	std::ostringstream oss;
	std::streambuf* old_buf;

	string READ = "read";
	string WRITE = "write";
	string ERASE = "erase";
	string FLUSH = "flush";
	string FULL_READ = "fullread";
	string FULL_WRITE = "fullwrite";
	string ERASE_RANGE = "erase_range";

	string LBA = "0";
	string FAIL_LBA = "100";
	string DEFAULT_DATA ="0x00000000";
	string ERROR_PATTERN = "ERROR";

	int MAX_LBA = 100;

	string getReadFormat(string lba, string expect) {
		string empty = "";
		if (lba.size() == 1)
		{
			empty = "0";
		}

		return "[Read] LBA " + empty + lba + " : "+ expect +"\n";
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

	string getEraseFormat() {
		return "[Erase] Done\n";
	}

	string getEraseErrorFormat() {
		return "[Erase] ERROR\n";
	}

	string getFlushFormat() {
		return "[Flush] Done\n";
	}


	string getEraseRangeFormat() {
		return "[EraseRange] Done\n";
	}

	string getEraseRangeErrorFormat() {
		return "[EraseRange] ERROR\n";
	}

	std::string intToHexString(int num) {
		const char* hexChars = "0123456789ABCDEF";
		std::string result = "0x";
		for (int i = 7; i >= 0; --i) {
			unsigned int nibble = (num >> (i * 4)) & 0xF;
			result += hexChars[nibble];
		}
		return result;
	}

private:
	void SetUp() override {
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

	mockCmdProcesser.run(commands);

	string str = oss.str();
	EXPECT_EQ(str, actual);
}

TEST_F(CommandProcesserFixture, ReadCommand_Fail) {
	vector<string> commands = { READ , LBA };
	string actual = "[Read] ERROR\n";
	

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Success) {
	vector<string> commands = { WRITE , LBA, DEFAULT_DATA };
	string actual = getWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_,_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Fail) {
	vector<string> commands = { WRITE , FAIL_LBA, DEFAULT_DATA };
	string actual = "[Write] ERROR\n";

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(1);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullReadCommand_Success) {
	vector<string> commands = { FULL_READ };
	string actual = getFullReadFormat();

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(100);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillRepeatedly(Return(DEFAULT_DATA));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullWriteCommand_Success) {
	vector<string> commands = { FULL_WRITE , DEFAULT_DATA};
	string actual = getFullWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(100);

	EXPECT_CALL(mockSssHandler, readOutput())
		.WillRepeatedly(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseCommand_Success) {
	vector<string> commands = { ERASE , LBA, "10" };
	string actual = getEraseFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseCommand_Fail) {
	vector<string> commands = { ERASE , LBA, "1000" };
	string actual = getEraseErrorFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return("ERROR"));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FlushCommand_Success) {
	vector<string> commands = { FLUSH };
	string actual = getFlushFormat();

	EXPECT_CALL(mockSssHandler, flush())
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseRageCommand_Success) {
	vector<string> commands = { ERASE_RANGE , LBA, "30" };
	string actual = getEraseRangeFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(3);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillRepeatedly(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

/*
TEST_F(CommandProcesserFixture, Real_WriteRead_Success) {
	for (int lba = 0; lba < MAX_LBA; lba++)
	{
		string writeData = intToHexString(lba);
		string sLba = to_string(lba);
		vector<string> writeCommands = { WRITE , sLba, writeData };
		vector<string> readCommands = { READ , sLba };
		
		string actual = getWriteFormat();
		actual += getReadFormat(sLba, writeData);

		realCmdProcesser.run(writeCommands);
		realCmdProcesser.run(readCommands);

		string str = oss.str();
		EXPECT_EQ(str, actual);
	}	
}
*/

