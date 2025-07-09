#include "gmock/gmock.h"
#include "ssd_client_app.h"
#include "command_processor.h"

using namespace testing;

class SsdClientAppFixture : public Test {
public:
	std::ostringstream oss;
	std::streambuf* old_buf;
	SsdHandlerMock mockSssHandler;

private:
	void SetUp() override {
		old_buf = std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(old_buf);
	}
};

TEST_F(SsdClientAppFixture, ReadSuccess) {
	EXPECT_CALL(mockSssHandler, read(_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return("0x00000000"));

	SsdClientApp app;
	app.setInputCmd("read 0");
	app.startVerify(&mockSssHandler);

	EXPECT_EQ(oss.str(), "[Read] LBA 0 : 0x00000000\n");
}

TEST_F(SsdClientAppFixture, WriteSuccess) {
	EXPECT_CALL(mockSssHandler, write(_,_))
		.Times(1);
	EXPECT_CALL(mockSssHandler, readOutput())
		.WillOnce(Return(""));

	SsdClientApp app;
	app.setInputCmd("write 3 0xAAAABBBB");
	app.startVerify(&mockSssHandler);

	EXPECT_EQ(oss.str(), "[Write] Done\n");
}