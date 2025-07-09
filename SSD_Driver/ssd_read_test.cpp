#include "gtest/gtest.h"
#include "ssd.h"
#include "fileio.h"
#include <fstream>
#include <cstdio>

using namespace testing;

const std::string test_nand_path = "test_nand.txt";

class SSDHandlerTestFixture : public Test {
public:
    const std::string TEST_NAN_PATH = "test_nand.txt";
    const std::string OUTPUT_FILE_PATH = "ssd_output.txt";
    const std::string ERROR = "ERROR";

    const int SSD_SIZE = 100;

    void writeOutput(const std::string& content) {
        std::ofstream outputFile(OUTPUT_FILE_PATH);
        outputFile << content;
    }

private:
    void SetUp() override {
        std::remove(TEST_NAN_PATH.c_str());
    }
    void TearDown() override {
        std::remove(TEST_NAN_PATH.c_str());
    }
};

TEST_F(SSDHandlerTestFixture, ReadSuccess) {
    std::ofstream test_file(TEST_NAN_PATH);
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 1 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 2 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 3 << "\n";
    test_file.close();

    FileInOut fileio(TEST_NAN_PATH);
    SSD handler(fileio.nandData);

    EXPECT_EQ(handler.doReadCmd(0), "0x00000001");
}

TEST_F(SSDHandlerTestFixture, ReadFail) {
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

TEST_F(SSDHandlerTestFixture, ReadInvalidAddress) {
    FileInOut fileio(TEST_NAN_PATH);
    SSD handler(fileio.nandData);

    EXPECT_EQ(handler.doReadCmd(100), "ERROR");
    EXPECT_EQ(handler.doReadCmd(-1), "ERROR");
}

