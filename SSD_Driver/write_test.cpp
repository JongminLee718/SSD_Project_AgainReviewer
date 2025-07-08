#include "gmock/gmock.h"
#include "ssd.h"
#include <string>

using namespace testing;

class WriteFixture : public Test {
public:
	SSD ssd;
	WriteFixture() {
		for (int addrIdx = 0; addrIdx < MAX_LBA; addrIdx++) {
			ssd.setData(addrIdx, 0);
		}
	}
	void writeTest(int tarAddr, int writeData) {
		std::string result = {};
		EXPECT_EQ(ssd.getData(tarAddr), 0);
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ(ssd.getData(tarAddr), writeData);
		EXPECT_EQ("", result);
	}
	void writeExpectionTest(int tarAddr, int writeData) {
		std::string result = {};
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ("ERROR", result);
	}

private:
	static const int MAX_LBA = 100;
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
