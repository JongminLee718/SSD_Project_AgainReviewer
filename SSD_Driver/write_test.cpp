#include "gmock/gmock.h"
#include "ssd.h"
#include "fileio.h"
#include <string>

using namespace testing;


class WriteFixture : public Test {
public:
	
	void writeTest(int tarAddr, int writeData) {
		FileInOut fileio(TEST_NAN_PATH);
		SSD ssd(fileio.nandData);
		std::string result = {};
		EXPECT_EQ(ssd.getData(tarAddr), 0);
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ(ssd.getData(tarAddr), writeData);
		EXPECT_EQ("", result);
	}
	void writeExpectionTest(int tarAddr, int writeData) {
		FileInOut fileio(TEST_NAN_PATH);
		SSD ssd(fileio.nandData);
		std::string result = {};
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ("ERROR", result);
	}

private:
	static const int MAX_LBA = 100;
	const std::string TEST_NAN_PATH = "test_nand.txt";
};

TEST_F(WriteFixture, writeAddr0) {
	writeTest(0, 0xAAA);
	writeTest(5, 0xCCC);
	writeTest(30, 0xCCC);
	writeTest(99, 0xCCC);
}

TEST_F(WriteFixture, writeExceptionAddress100) {

	writeExpectionTest(100, 0xCCC);
	writeExpectionTest(-1, 0xCCC);
}
