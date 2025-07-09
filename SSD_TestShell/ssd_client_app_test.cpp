#include "gmock/gmock.h"
#include "ssd_client_app.h"
#include "command_processor.h"
#include <string>

using namespace testing;
using std::string;
using std::to_string;

class SsdClientAppFixture : public Test {
public:
	SsdHandlerMock mockSsdHandler;
	OutputCheckerMock mockChecker;
	SsdClientApp app;

	std::ostringstream oss;
	std::streambuf* old_buf;

	const string USER_READ_CMD = "read 0";
	const string USER_WRITE_CMD = "write 3 0xAAAABBBB";
	const string USER_WRITE_CMD_FAIL = "write 100 0xAAAABBBB";
	const string USER_FULL_READ_CMD = "fullread";
	const string USER_FULL_WRITE_CMD = "fullwrite 0x00000000";
	const string USER_SCRIPT1_CMD = "1_";
	const string USER_SCRIPT2_CMD = "2_";
	const string USER_SCRIPT3_CMD = "3_";

	const string LBA = "0";
	const string DEFAULT_DATA = "0x00000000";
	const string ERROR_PATTERN = "ERROR";

	int MAX_LBA = 100;

	string getReadFormat(string lba, string expect) {
		string empty = "";
		if (lba.size() == 1)
		{
			empty = "0";
		}

		return "[Read] LBA " + empty + lba + " : " + expect + "\n";
	}

	string getWriteFormat() {
		return "[Write] Done\n";
	}

	string getReadFormatForFullRead(string lba, string expect) {
		string empty = "";
		if (lba.size() == 1)
		{
			empty = "0";
		}

		return "LBA " + empty + lba + " : " + expect + "\n";
	}

	string getFullReadFormat() {
		string result = "[Full Read]\n";
		for (int lba = 0; lba < MAX_LBA; lba++) {
			result = result + getReadFormatForFullRead(to_string(lba), DEFAULT_DATA);
		}
		return result;
	}

	string getFullWriteFormat() {
		return "[Full Write] Done\n";
	}

	void doClientApp(string userCmd) {
		app.setInputCmd(userCmd);
		app.startVerify(&mockSsdHandler, &mockChecker);
	}

private:
	void SetUp() override {
		old_buf = std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(old_buf);
	}
};

TEST_F(SsdClientAppFixture, ReadCommand_Success) {
	string actual = getReadFormat(LBA, DEFAULT_DATA);

	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(1);
	EXPECT_CALL(mockSsdHandler, readOutput())
		.WillOnce(Return(DEFAULT_DATA));

	doClientApp(USER_READ_CMD);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(SsdClientAppFixture, WriteCommand_Success) {
	string actual = getWriteFormat();

	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(1);
	EXPECT_CALL(mockSsdHandler, readOutput())
		.WillOnce(Return(""));

	doClientApp(USER_WRITE_CMD);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(SsdClientAppFixture, FullReadCommand_Success) {
	string actual = getFullReadFormat();

	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(100);

	EXPECT_CALL(mockSsdHandler, readOutput())
		.WillRepeatedly(Return(DEFAULT_DATA));

	doClientApp(USER_FULL_READ_CMD);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(SsdClientAppFixture, FullWriteCommand_Success) {
	string actual = getFullWriteFormat();

	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(100);

	EXPECT_CALL(mockSsdHandler, readOutput())
		.WillRepeatedly(Return(""));

	doClientApp(USER_FULL_WRITE_CMD);

	EXPECT_EQ(oss.str(), actual);
}

TEST_F(SsdClientAppFixture, FullWriteAndReadComapreCommand_Test) {
	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(100);
	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(100);
	EXPECT_CALL(mockChecker, outputChecker(_))
		.Times(100)
		.WillRepeatedly(Return(true));

	doClientApp(USER_SCRIPT1_CMD);

	EXPECT_EQ(oss.str(), "PASS\n");
}

TEST_F(SsdClientAppFixture, PartialLBAWrite_Test) {
	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(150);
	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(150);
	EXPECT_CALL(mockChecker, outputChecker(_))
		.Times(150)
		.WillRepeatedly(Return(true));

	doClientApp(USER_SCRIPT2_CMD);

	EXPECT_EQ(oss.str(), "PASS\n");
}

TEST_F(SsdClientAppFixture, WriteReadAgingCommand_Test) {
	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(400);
	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(400);
	EXPECT_CALL(mockChecker, outputChecker(_))
		.Times(400)
		.WillRepeatedly(Return(true));

	doClientApp(USER_SCRIPT3_CMD);

	EXPECT_EQ(oss.str(), "PASS\n");
}