#include "gmock/gmock.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "buffer_manager.h"
#include "ssd.h"
#include "fileio.h"

namespace fs = std::filesystem;
using namespace testing;

class BufferManagerFixture : public Test {
public:
    const std::string test_nand_path = "nand.txt";
    const std::string bufferDir = "buffer";

    void SetUp() override {
        std::remove(test_nand_path.c_str());
        if (fs::exists(bufferDir)) {
            fs::remove_all(bufferDir);
        }
        fs::create_directory(bufferDir);
    }

    void TearDown() override {
        std::remove(test_nand_path.c_str());
        if (fs::exists(bufferDir)) {
            fs::remove_all(bufferDir);
        }
    }

    void initializeNandFile(unsigned int value, int size = 100) {
        std::ofstream nandFile(test_nand_path);
        for (int i = 0; i < size; ++i) {
            nandFile << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value << "\n";
        }
        nandFile.close();
    }

    int countBufferFiles() {
        int count = 0;
        if (fs::exists(bufferDir)) {
            for (const auto& entry : fs::directory_iterator(bufferDir)) {
                count++;
            }
        }
        return count;
    }

    std::vector<std::string> getBufferCommands() {
        std::vector<std::string> commands;
        if (fs::exists(bufferDir)) {
            for (const auto& entry : fs::directory_iterator(bufferDir)) {
                std::string filename = entry.path().filename().string();
                if (filename.find("_empty") == std::string::npos) {
                    commands.push_back(filename);
                }
            }
        }
        std::sort(commands.begin(), commands.end()); 
        return commands;
    }
};

TEST_F(BufferManagerFixture, InitializeBuffer_CreatesDirectoryAndFiles) {
    fs::remove_all(bufferDir);
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    ASSERT_TRUE(fs::exists(bufferDir));
    ASSERT_EQ(countBufferFiles(), 5);
}

TEST_F(BufferManagerFixture, AddCommand_SimpleWrite) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 20, 0x12345678);
    unsigned int readValue;
    ASSERT_TRUE(bm.readFromBuffer(20, readValue));
    EXPECT_EQ(readValue, 0x12345678);
}

TEST_F(BufferManagerFixture, AddCommand_OverwriteWrite) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 10, 0x11111111);
    bm.addCommandInBuffer("W", 10, 0xAAAAAAAA);
    unsigned int readValue;
    ASSERT_TRUE(bm.readFromBuffer(10, readValue));
    EXPECT_EQ(readValue, 0xAAAAAAAA);
}

TEST_F(BufferManagerFixture, ReadFromBuffer_AfterErase) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 30, 0x12345678);
    bm.addCommandInBuffer("E", 30, 1);
    unsigned int readValue;
    ASSERT_TRUE(bm.readFromBuffer(30, readValue));
    EXPECT_EQ(readValue, 0x00000000);
}

TEST_F(BufferManagerFixture, ReadFromBuffer_WriteAfterErase) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 40, 3);
    bm.addCommandInBuffer("W", 41, 0xDEADBEEF);
    unsigned int readValue;
    ASSERT_TRUE(bm.readFromBuffer(40, readValue));
    EXPECT_EQ(readValue, 0x00000000);
    ASSERT_TRUE(bm.readFromBuffer(41, readValue));
    EXPECT_EQ(readValue, 0xDEADBEEF);
    ASSERT_TRUE(bm.readFromBuffer(42, readValue));
    EXPECT_EQ(readValue, 0x00000000);
}

TEST_F(BufferManagerFixture, ReadFromBuffer_LbaNotFound) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 1, 0x11111111);
    unsigned int readValue;
    EXPECT_FALSE(bm.readFromBuffer(99, readValue));
}

TEST_F(BufferManagerFixture, Flush_EmptyBuffer) {
    initializeNandFile(0xAAAAAAAA);
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[0], 0xAAAAAAAA);
    auto commands = getBufferCommands();
    EXPECT_TRUE(commands.empty());
}

TEST_F(BufferManagerFixture, AutoFlush_WhenBufferIsFull) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 0, 0x11111111);
    bm.addCommandInBuffer("W", 1, 0x22222222);
    bm.addCommandInBuffer("W", 2, 0x33333333);
    bm.addCommandInBuffer("W", 3, 0x44444444);
    bm.addCommandInBuffer("W", 4, 0x55555555);
    bm.addCommandInBuffer("W", 5, 0x66666666);
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[0], 0x11111111);
    EXPECT_EQ(fileio.nandData[4], 0x55555555);
    EXPECT_EQ(fileio.nandData[5], 0x00000000);
    unsigned int readValue;
    EXPECT_TRUE(bm.readFromBuffer(5, readValue));
    EXPECT_EQ(readValue, 0x66666666);
    EXPECT_FALSE(bm.readFromBuffer(0, readValue));
}

TEST_F(BufferManagerFixture, Flush_WriteIsOverwrittenByErase) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 55, 0x12345678);
    bm.addCommandInBuffer("E", 55, 1);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[55], 0x00000000);
}

TEST_F(BufferManagerFixture, Flush_EraseIsPartiallyOverwrittenByWrite) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 70, 5);
    bm.addCommandInBuffer("W", 72, 0xDEADBEEF);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[70], 0x00000000);
    EXPECT_EQ(fileio.nandData[71], 0x00000000);
    EXPECT_EQ(fileio.nandData[72], 0xDEADBEEF);
    EXPECT_EQ(fileio.nandData[73], 0x00000000);
    EXPECT_EQ(fileio.nandData[74], 0x00000000);
}

TEST_F(BufferManagerFixture, Flush_ComplexInteractions) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 10, 0x11111111);
    bm.addCommandInBuffer("E", 20, 3);
    bm.addCommandInBuffer("W", 10, 0xAAAAAAAA);
    bm.addCommandInBuffer("W", 21, 0xBBBBBBBB);
    bm.addCommandInBuffer("W", 99, 0xFFFFFFFF);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[10], 0xAAAAAAAA);
    EXPECT_EQ(fileio.nandData[20], 0x00000000);
    EXPECT_EQ(fileio.nandData[21], 0xBBBBBBBB);
    EXPECT_EQ(fileio.nandData[22], 0x00000000);
    EXPECT_EQ(fileio.nandData[99], 0xFFFFFFFF);
}


TEST_F(BufferManagerFixture, EraseMerge_Adjacent) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 10, 2); 
    bm.addCommandInBuffer("E", 12, 3); 
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("E_10_5"), std::string::npos);
}

TEST_F(BufferManagerFixture, EraseMerge_Overlapping) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 10, 5);
    bm.addCommandInBuffer("E", 13, 4);
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("E_10_7"), std::string::npos);
}


TEST_F(BufferManagerFixture, EraseContain_NewIsIgnored) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 20, 10); 
    bm.addCommandInBuffer("E", 22, 3); 
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("E_20_10"), std::string::npos);
}

TEST_F(BufferManagerFixture, EraseContain_OldIsReplaced) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 22, 3);
    bm.addCommandInBuffer("E", 20, 10); 
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("E_20_10"), std::string::npos);
}

TEST_F(BufferManagerFixture, EraseHeal_AfterSplitByWrite) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 30, 10);
    bm.addCommandInBuffer("W", 35, 0xABCDEF01);
    bm.addCommandInBuffer("E", 35, 1);
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("E_30_10"), std::string::npos);
}

TEST_F(BufferManagerFixture, WriteTrimsErase_AtStart) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 40, 5);
    bm.addCommandInBuffer("W", 40, 0x11111111);
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 2);
    EXPECT_NE(commands[0].find("E_41_4"), std::string::npos);
    EXPECT_NE(commands[1].find("W_40_0x11111111"), std::string::npos);
}

TEST_F(BufferManagerFixture, WriteTrimsErase_AtEnd) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 50, 5);
    bm.addCommandInBuffer("W", 54, 0x22222222);
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 2);
    EXPECT_NE(commands[0].find("E_50_4"), std::string::npos);
    EXPECT_NE(commands[1].find("W_54_0x22222222"), std::string::npos);
}

TEST_F(BufferManagerFixture, SplitIsSkipped_WhenBufferIsFull) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 1, 1);
    bm.addCommandInBuffer("W", 2, 2);
    bm.addCommandInBuffer("W", 3, 3);
    bm.addCommandInBuffer("W", 4, 4); 
    bm.addCommandInBuffer("E", 80, 10);
    bm.addCommandInBuffer("W", 85, 0xABC); 
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_NE(commands[0].find("W_85_0x00000ABC"), std::string::npos);
}

TEST_F(BufferManagerFixture, SplitAfterMergeTest) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 70, 4);
    bm.addCommandInBuffer("E", 73, 4); 
    bm.addCommandInBuffer("W", 75, 0xCCCCCCCC); 
    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 3);
    EXPECT_NE(commands[0].find("E_70_5"), std::string::npos);
    EXPECT_NE(commands[1].find("E_76_1"), std::string::npos);
    EXPECT_NE(commands[2].find("W_75_0xCCCCCCCC"), std::string::npos);
}

TEST_F(BufferManagerFixture, Flush_AfterEraseMerge) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 10, 5); 
    bm.addCommandInBuffer("E", 14, 10);
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 10; i < 17; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
}

TEST_F(BufferManagerFixture, EraseMergeWithOverSize) {
    initializeNandFile(0xFFFFFFFF);
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 1, 0xCCCCCCCC);
    bm.addCommandInBuffer("W", 2, 0xCCCCCCCC);
    bm.addCommandInBuffer("W", 3, 0xCCCCCCCC);
    bm.addCommandInBuffer("W", 4, 0xCCCCCCCC);
    bm.addCommandInBuffer("E", 10, 5); //10 11 12 13 14
    bm.addCommandInBuffer("E", 14, 9); //14 15 16 17 18 19 20 21 22

    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 10; i < 17; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    } 
}

TEST_F(BufferManagerFixture, Flush_AfterEraseContainment) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 20, 10);
    bm.addCommandInBuffer("E", 22, 3);
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 20; i < 30; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
}

TEST_F(BufferManagerFixture, Flush_AfterWriteSplitsErase) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 30, 10);
    bm.addCommandInBuffer("W", 35, 0xDEADBEEF);
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 30; i < 35; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
    EXPECT_EQ(fileio.nandData[35], 0xDEADBEEF);
    for (int i = 36; i < 40; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
}

TEST_F(BufferManagerFixture, Flush_AfterWriteTrimsEraseAtStart) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 40, 5);
    bm.addCommandInBuffer("W", 40, 0x11111111);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[40], 0x11111111);
    for (int i = 41; i < 45; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
}

TEST_F(BufferManagerFixture, Flush_AfterWriteTrimsEraseAtEnd) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 50, 5);
    bm.addCommandInBuffer("W", 54, 0x22222222);
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 50; i < 54; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
    EXPECT_EQ(fileio.nandData[54], 0x22222222);
}

TEST_F(BufferManagerFixture, Flush_AfterEraseHealsSplit) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 60, 10);
    bm.addCommandInBuffer("W", 65, 0xABC);
    bm.addCommandInBuffer("E", 65, 1); 
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 60; i < 70; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
}

TEST_F(BufferManagerFixture, Flush_OnNonZeroNand) {
    initializeNandFile(0xFFFFFFFF);
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 5, 0xAAAAAAAA);
    bm.addCommandInBuffer("E", 10, 2);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[4], 0xFFFFFFFF);
    EXPECT_EQ(fileio.nandData[5], 0xAAAAAAAA);
    EXPECT_EQ(fileio.nandData[6], 0xFFFFFFFF);
    EXPECT_EQ(fileio.nandData[10], 0x00000000);
    EXPECT_EQ(fileio.nandData[11], 0x00000000);
    EXPECT_EQ(fileio.nandData[12], 0xFFFFFFFF);
}

TEST_F(BufferManagerFixture, Flush_ManualAfterAutoFlush) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 0, 0x1);
    bm.addCommandInBuffer("W", 1, 0x2);
    bm.addCommandInBuffer("W", 2, 0x3);
    bm.addCommandInBuffer("W", 3, 0x4);
    bm.addCommandInBuffer("W", 4, 0x5);
    bm.addCommandInBuffer("W", 10, 0xA);
    bm.addCommandInBuffer("W", 11, 0xB);
    bm.flush();
    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[0], 0x1);
    EXPECT_EQ(fileio.nandData[4], 0x5);
    EXPECT_EQ(fileio.nandData[10], 0xA);
    EXPECT_EQ(fileio.nandData[11], 0xB);
}
  
TEST_F(BufferManagerFixture, Flush_AfterSplitAfterMerge) {
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("E", 90, 4);
    bm.addCommandInBuffer("E", 93, 4);
    bm.addCommandInBuffer("W", 95, 0xCCCCCCCC);
    bm.flush();
    FileInOut fileio(test_nand_path);
    for (int i = 90; i < 95; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }
    EXPECT_EQ(fileio.nandData[95], 0xCCCCCCCC);
    EXPECT_EQ(fileio.nandData[96], 0x00000000);
}

TEST_F(BufferManagerFixture,Flush_WhenSplitRequiresMoreSlots) {
    initializeNandFile(0xFFFFFFFF);
    BufferManager bm(test_nand_path);
    bm.initializeBuffer();
    bm.addCommandInBuffer("W", 1, 1);
    bm.addCommandInBuffer("W", 2, 2);
    bm.addCommandInBuffer("W", 3, 3);
    bm.addCommandInBuffer("E", 10, 10);
    bm.addCommandInBuffer("W", 15, 0xABC);

    FileInOut fileio(test_nand_path);
    EXPECT_EQ(fileio.nandData[1], 1);
    EXPECT_EQ(fileio.nandData[2], 2);
    EXPECT_EQ(fileio.nandData[3], 3);
    for (int i = 10; i < 20; ++i) {
        EXPECT_EQ(fileio.nandData[i], 0x00000000) << "at LBA " << i;
    }

    auto commands = getBufferCommands();
    ASSERT_EQ(commands.size(), 1);
    EXPECT_TRUE(commands[0].find("W_15_0x00000ABC"));
}