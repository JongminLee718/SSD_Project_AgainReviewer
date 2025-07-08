#include "gmock/gmock.h"
#include "ssd_verify_app.h"
#include "command_processer.h"

using namespace testing;

class SsdVerifyAppFixture : public Test {
public:
	std::ostringstream oss;
	std::streambuf* old_buf;

private:
	void SetUp() override {
		old_buf = std::cout.rdbuf(oss.rdbuf());
	}

	void TearDown() override {
		std::cout.rdbuf(old_buf);
	}
};

TEST_F(SsdVerifyAppFixture, ReadSuccess) {
	SsdVerifyApp app;
	app.setInputCmd("read 0");
	app.startVerify();

	EXPECT_EQ(oss.str(), "[Read] LBA 0 : 0x00000000\n");
}

TEST_F(SsdVerifyAppFixture, WriteSuccess) {
	SsdVerifyApp app;
	app.setInputCmd("write 3 0xAAAABBBB");
	app.startVerify();

	EXPECT_EQ(oss.str(), "[Write] Done\n");
}