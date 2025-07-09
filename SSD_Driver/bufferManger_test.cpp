#include "gmock/gmock.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include "bufferManager.cpp"
#include "ssd.h"

const std::string bufferDir = "buffer";
const std::string test_nand_path = "test_nand.txt";

namespace fs = std::filesystem;
using namespace testing;

class BufferManagerFixture : public Test {
public:
	void create_buffer_file() {
		if (!fs::exists(bufferDir)) {
			fs::create_directory(bufferDir);
		}

		for (int i = 1; i <= 5; ++i) {
			std::string init_file_name = std::to_string(i) + "_empty";
			std::ofstream(fs::path(bufferDir) / init_file_name);

		}
	}

	int check_buffer_file() {
		int fileCnt = 0;
		for (int i = 1; i <= 5; i++) {
			for (const auto& entry : std::filesystem::directory_iterator(bufferDir)) {
				if (entry.path().filename().string().starts_with(std::to_string(i) + "_")) {
					fileCnt++;
					break;
				}
			}
		}
		return fileCnt;
	}

	bool check_file_exists(bool isExist) {
		std::string expected = "1_W_20_0x12345678";

		for (const auto& entry : fs::directory_iterator(bufferDir)) {
			std::string actual = entry.path().filename().string();
			if (expected == actual) isExist = true;

		}
		return isExist;
	}
private:
	void SetUp() override {
		std::remove(test_nand_path.c_str());
	}
	void TearDown() override {
		std::remove(test_nand_path.c_str());
	}
};

TEST_F(BufferManagerFixture, test_check_initialize_file_create) {
	create_buffer_file();
	int expected = 5;
	int actual = check_buffer_file();
	EXPECT_EQ(expected, actual);
}

TEST_F(BufferManagerFixture, test_check_file_exists_after_adding_to_buffer) {
	BufferManager bm(test_nand_path);
	bm.initializeBuffer();
	bm.addCommandInBuffer("W", 20, 0x12345678);
	bool isExist = false;
	EXPECT_TRUE(check_file_exists(isExist));

}

TEST_F(BufferManagerFixture, test_check_file_unexists_after_adding_to_buffer) {
	BufferManager bm(test_nand_path);
	bm.initializeBuffer();
	bm.addCommandInBuffer("W", 20, 0x99999999);
	bool isExist = false;
	EXPECT_FALSE(check_file_exists(isExist));
}

TEST_F(BufferManagerFixture, test_read_from_buffer) {
	BufferManager bm(test_nand_path);
	bm.initializeBuffer();
	bm.addCommandInBuffer("W", 20, 0x99999999);
	unsigned int result;
	bool find = bm.readFromBuffer(20, result);
	std::stringstream ss;
	if (find) {
		ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << result;
	}
	std::string actual = ss.str();
	std::string expected = "0x99999999";
	EXPECT_EQ(expected, actual);
}

TEST_F(BufferManagerFixture, test_flush) {
	BufferManager bm(test_nand_path);
	bm.initializeBuffer();
	bm.addCommandInBuffer("W", 1, 0x99991111);
	bm.flush();
	FileInOut fileio(test_nand_path);
	SSD handler(fileio.nandData);
	std::string actual = handler.doReadCmd(1);
	std::string expected = "0x99991111";
	EXPECT_EQ(expected, actual);
}