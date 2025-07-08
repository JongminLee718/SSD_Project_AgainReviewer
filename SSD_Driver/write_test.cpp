#include "gmock/gmock.h"
#include "ssd.h"

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
		int result = 0;
		EXPECT_EQ(ssd.getData(tarAddr), 0);
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ(ssd.getData(tarAddr), writeData);
		EXPECT_EQ(1, result);
	}
	void writeExpectionTest(int tarAddr, int writeData) {
		int result = 0;
		result = ssd.doWriteCmd(tarAddr, writeData);
		EXPECT_EQ(0, result);
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
