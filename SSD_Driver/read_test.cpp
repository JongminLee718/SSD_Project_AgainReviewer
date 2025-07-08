#include "gtest/gtest.h"
#include "ssdHandler.h"
#include <fstream>
#include <cstdio>

const std::string test_nand_path = "test_nand.txt";

TEST(SSDHandlerTest, ReadSuccess) {
    std::ofstream test_file(test_nand_path);
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 1 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 2 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 3 << "\n";
    test_file.close();

    SSDHandler handler(test_nand_path);

    EXPECT_EQ(handler.executeRead(0), "0x00000001");
    EXPECT_EQ(handler.executeRead(1), "0x00000002");
    EXPECT_EQ(handler.executeRead(2), "0x00000003");
}

TEST(SSDHandlerTest, ReadFail) {
    std::ofstream test_file(test_nand_path);
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 2 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 3 << "\n";
    test_file << "0x" << std::hex << std::setw(8) << std::setfill('0') << 4 << "\n";
    test_file.close();

    SSDHandler handler(test_nand_path);

    EXPECT_NE(handler.executeRead(0), "0x00000001");
    EXPECT_NE(handler.executeRead(1), "0x00000002");
    EXPECT_NE(handler.executeRead(2), "0x00000003");
}

TEST(SSDHandlerTest, ReadInvalidAddress) {
    SSDHandler handler(test_nand_path);

    EXPECT_EQ(handler.executeRead(100), "ERROR");
    EXPECT_EQ(handler.executeRead(-1), "ERROR");
}

