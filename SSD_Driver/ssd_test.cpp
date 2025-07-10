#include <fstream>
#include <string>
#include "gmock/gmock.h"
#include "ssd.h"
#include "fileio.h"
#include "main.h"

using namespace testing;

class SsdFixture : public Test {
public:
	const int SSD_SIZE = 100;
	void writeOutput(const std::string& content) {
		std::ofstream outputFile(OUTPUT_FILE_PATH);
		outputFile << content;
	}
	
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
	void SetUp() override {
		std::remove(TEST_NAN_PATH.c_str());
	}
	void TearDown() override {
		std::remove(TEST_NAN_PATH.c_str());
	}
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

TEST_F(SsdFixture, ReadSuccess) {
	std::ofstream test_file(TEST_NAN_PATH);
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 1 << "\n";
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 2 << "\n";
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 3 << "\n";
	test_file.close();

	FileInOut fileio(TEST_NAN_PATH);
	SSD handler(fileio.nandData);

	EXPECT_EQ(handler.doReadCmd(0), "0x00000001");
}

TEST_F(SsdFixture, ReadFail) {
	std::ofstream test_file(TEST_NAN_PATH);
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 2 << "\n";
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 3 << "\n";
	test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 4 << "\n";
	test_file.close();

	FileInOut fileio(TEST_NAN_PATH);
	SSD handler(fileio.nandData);

	EXPECT_NE(handler.doReadCmd(0), "0x00000001");
	EXPECT_NE(handler.doReadCmd(1), "0x00000002");
	EXPECT_NE(handler.doReadCmd(2), "0x00000003");
}

TEST_F(SsdFixture, ReadInvalidAddress) {
	FileInOut fileio(TEST_NAN_PATH);
	SSD handler(fileio.nandData);

	EXPECT_EQ(handler.doReadCmd(100), "ERROR");
	EXPECT_EQ(handler.doReadCmd(-1), "ERROR");
}

