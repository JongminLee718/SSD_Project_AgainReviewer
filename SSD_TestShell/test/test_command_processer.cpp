#include "gmock/gmock.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "../utils.h"
#include "../command_processor.h"
#include <format>

using namespace testing;
using std::string;
using std::vector;
using std::to_string;

class CommandProcesserFixture : public Test {
public:
	UtilsMock mockUtils;
	SsdHandlerMock mockSssHandler;

	CommandProcessor mockCmdProcesser{ &mockSssHandler, &mockUtils };

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

		if (lba.size() == 1) {
			empty = "0";
		}

		return "[Read] LBA " + empty + lba + " : "+ expect +"\n";
	}

	string getWriteFormat() {
		return "[Write] Done\n";
	}

	string getReadFormatForFullRead(string lba, string expect) {
		string empty = "";

		if (lba.size() == 1) {
			empty = "0";
		}

		return "LBA " + empty + lba + " : " + expect + "\n";
	}

	string getFullReadFormat() {
		string result="[Full Read]\n";
		for (int lba = 0; lba < MAX_LBA; lba++) {
			result = result + getReadFormatForFullRead(to_string(lba), DEFAULT_DATA);
		}
		return result;
	}

	string getFullWriteFormat() {
		return "[Full Write] Done\n";
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
	EXPECT_CALL(mockUtils, readOutput())
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
	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Success) {
	vector<string> commands = { WRITE , LBA, DEFAULT_DATA };
	string actual = getWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_,_))
		.Times(1);
	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, WriteCommand_Fail) {
	vector<string> commands = { WRITE , FAIL_LBA, DEFAULT_DATA };
	string actual = "[Write] ERROR\n";

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(1);

	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return(ERROR_PATTERN));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullReadCommand_Success) {
	vector<string> commands = { FULL_READ };
	string actual = getFullReadFormat();

	EXPECT_CALL(mockSssHandler, read(_))
		.Times(100);

	EXPECT_CALL(mockUtils, readOutput())
		.WillRepeatedly(Return(DEFAULT_DATA));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FullWriteCommand_Success) {
	vector<string> commands = { FULL_WRITE , DEFAULT_DATA};
	string actual = getFullWriteFormat();

	EXPECT_CALL(mockSssHandler, write(_, _))
		.Times(100);

	EXPECT_CALL(mockUtils, readOutput())
		.WillRepeatedly(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseCommand_Success) {
	vector<string> commands = { ERASE , LBA, "10" };
	string actual = getEraseFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(1);
	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseCommand_Fail) {
	vector<string> commands = { ERASE , LBA, "1000" };
	string actual = getEraseErrorFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(1);
	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return("ERROR"));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, FlushCommand_Success) {
	vector<string> commands = { FLUSH };
	string actual = getFlushFormat();

	EXPECT_CALL(mockSssHandler, flush())
		.Times(1);
	EXPECT_CALL(mockUtils, readOutput())
		.WillOnce(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(CommandProcesserFixture, EraseRageCommand_Success) {
	vector<string> commands = { ERASE_RANGE , LBA, "30" };
	string actual = getEraseRangeFormat();

	EXPECT_CALL(mockSssHandler, erase(_, _))
		.Times(4);
	EXPECT_CALL(mockUtils, readOutput())
		.WillRepeatedly(Return(""));

	mockCmdProcesser.run(commands);

	EXPECT_EQ(oss.str(), actual);
}

#if (0)
TEST_F(CommandProcesserFixture, Random_Success) {
	Utils utils;
	SsdHandler ssdHandler;
	CommandProcessor cmdProcesser{ &ssdHandler, &utils };
	int dataMap[100] = { 0, };

	std::random_device rd;
	std::mt19937 gen(rd());

	int maxRandome = 500;
	int readMar = 100;
	std::uniform_int_distribution<> dis(0, maxRandome);
	std::uniform_int_distribution<> lbaDis(0, 99);
	std::uniform_int_distribution<> dataDis(0, 1000);
	std::stringstream ss;
	vector<string> cmd = { "erase_range", "0", "99" };

	cmdProcesser.run(cmd);

	for (int index = 0; index < 1000; index++) {
		int random_value = dis(gen);
		int lba = lbaDis(gen);

		if (readMar > random_value) {
			//read
			cmd = { "read", to_string(lba) };
			cmdProcesser.run(cmd);
			string result = utils.readOutput();

			cout << "read  " << to_string(lba) << " " << result << "\n";
			EXPECT_EQ(dataMap[lba], std::stoi(result, nullptr, 16));
			while (dataMap[lba] != std::stoi(result, nullptr, 16))
			{
				int a = 0;
			}
		}
		else if (maxRandome > random_value) {
			//write
			int data = dataDis(gen);

			std::string hex_data = "0x" + std::format("{:x}", data);
			cmd = { "write", to_string(lba), hex_data };

			dataMap[lba] = data;
			cmdProcesser.run(cmd);

			cout << "write  " << to_string(lba) << " " << hex_data << "\n";

		}
		else if (maxRandome == random_value) {

			//erase
			std::uniform_int_distribution<> endlbaDis(lba, 99);
			int endlba = endlbaDis(gen);

			cmd = { "erase_range", to_string(lba), to_string(endlba) };

			cmdProcesser.run(cmd);
			cout << "erase  " << to_string(lba) << " " << to_string(endlba) << "\n";

			for (int clba = lba; clba <= endlba; clba++) {
				dataMap[clba] = 0;
			}

		}
		else {
			FAIL();
		}

	}

	//check
	for (int lba = 0; lba < 100; lba++) {
		vector<string> cmd = { "read", to_string(lba) };
		cmdProcesser.run(cmd);
		string result = utils.readOutput();

		EXPECT_EQ(dataMap[lba], std::stoi(result, nullptr, 16));
		while (dataMap[lba] != std::stoi(result, nullptr, 16))
		{
			int a = 0;
		}
	}
}
#endif