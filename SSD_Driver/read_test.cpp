#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "InterfaceStorage.h"
#include "reader.h"
#include <string>

const std::string NAND_FILE_PATH = "ssd_nand.txt";
const std::string OUTPUT_FILE_PATH = "ssd_output.txt";

using namespace testing;

class MockStorage : public InterfaceStorage {
public:
    MOCK_METHOD(bool, read, (int lba, unsigned int& result), (override));
};

TEST(ApplicationTest, ReadFailWhenLbaIsInvalid) {
    MockStorage mockStorage;
    ssdProgram ssd_reader(&mockStorage);

    EXPECT_CALL(mockStorage, read(100, _))
        .Times(1)
        .WillOnce(Return(false));

    std::string result = ssd_reader.executeRead(100);
    EXPECT_EQ(result, "ERROR");
}