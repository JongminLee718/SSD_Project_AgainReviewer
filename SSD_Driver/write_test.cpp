#include "gmock/gmock.h"
#include "ssd.h"
#include "fileio.h"
#include <string>

using namespace testing;


class SsdFixture : public Test {
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
	void EraseTest(int tarAddr, int eraseSize) {
		FileInOut fileio(TEST_NAN_PATH);
		SSD ssd(fileio.nandData);
		std::string result = {};
		result = ssd.doEraseCmd(tarAddr, eraseSize);
		for (int addrIdx = 0;addrIdx < eraseSize;addrIdx++) {
			EXPECT_EQ(0, ssd.getData(tarAddr + addrIdx));
			EXPECT_EQ("", result);
		}
	}
	void EraseExceptionTest(int tarAddr, int eraseSize) {
		FileInOut fileio(TEST_NAN_PATH);
		SSD ssd(fileio.nandData);
		std::string result = {};
		result = ssd.doEraseCmd(tarAddr, eraseSize);
		EXPECT_EQ("ERROR", result);
	}

private:
	static const int MAX_LBA = 100;
	const std::string TEST_NAN_PATH = "test_nand.txt";
};

TEST_F(SsdFixture, writeAddr0) {
	writeTest(0, 0xAAA);
	writeTest(5, 0xCCC);
	writeTest(30, 0xCCC);
	writeTest(99, 0xCCC);
}

TEST_F(SsdFixture, writeExceptionAddress100) {

	writeExpectionTest(100, 0xCCC);
	writeExpectionTest(-1, 0xCCC);
}

TEST_F(SsdFixture, Erase) {
	EraseTest(0, 1);
	EraseTest(0, 10);
	EraseTest(30, 2);
	EraseTest(50, 5);
	EraseTest(23, 10);
	EraseTest(70, 10);
	EraseTest(90, 10);
	EraseTest(65, 1);
}

TEST_F(SsdFixture, EraseException) {
	EraseExceptionTest(0, 15);
	EraseExceptionTest(0, 0);
	EraseExceptionTest(70, 15);
	EraseExceptionTest(95, 10);
}
