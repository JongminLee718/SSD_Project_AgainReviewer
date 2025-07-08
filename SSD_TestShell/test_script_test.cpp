#include "command_mock.h"
#include "shell_command.h"
#include "test_script.cpp"

#include <iostream>
#include <sstream>


using namespace testing;

TEST(TS, FullWriteAndReadComapre_Test1) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// Command operation count check
	EXPECT_CALL(cm, run())
		.Times(200)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(cm, set(_))
		.Times(200);

	EXPECT_CALL(cc, nandOutputChecker(_))
		.WillRepeatedly(Return(true));

	ts.fullWriteAndReadCompare();

	// PASS string check
	EXPECT_EQ(oss.str(), "PASS");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, FullWriteAndReadComapre_Test2) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// run fail check
	EXPECT_CALL(cm, set(_));
	EXPECT_CALL(cm, run())
		.WillRepeatedly(Return(false));

	EXPECT_CALL(cc, nandOutputChecker(_))
		.WillRepeatedly(Return(true));

	ts.fullWriteAndReadCompare();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, FullWriteAndReadComapre_Test3) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// nandOutputChecker fail check
	EXPECT_CALL(cm, set(_))
		.Times(2);
	EXPECT_CALL(cm, run())
		.WillRepeatedly(Return(true));
	
	EXPECT_CALL(cc, nandOutputChecker(_))
		.WillRepeatedly(Return(false));

	ts.fullWriteAndReadCompare();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, PartalLBAWrite_Test1) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// Command operation count check
	EXPECT_CALL(cm, run())
		.Times(150)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(cm, set(_))
		.Times(150);

	EXPECT_CALL(cc, nandReadAndChecker(_, _))
		.Times(150)
		.WillRepeatedly(Return(true));

	ts.partialLBAWrite();

	// PASS string check
	EXPECT_EQ(oss.str(), "PASS");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, PartalLBAWrite_Test2) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// run fail check
	EXPECT_CALL(cm, set(_));
	EXPECT_CALL(cm, run())
		.WillRepeatedly(Return(false));
	
	ts.partialLBAWrite();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, PartalLBAWrite_Test3) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// run fail check
	EXPECT_CALL(cm, set(_))
		.Times(5);
	EXPECT_CALL(cm, run())
		.Times(5)
		.WillRepeatedly(Return(true));
	
	EXPECT_CALL(cc, nandReadAndChecker(_, _))
		.WillRepeatedly(Return(false));

	ts.partialLBAWrite();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, WriteReadAging_Test1) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// Command operation count check
	EXPECT_CALL(cm, set(_))
		.Times(400);
	EXPECT_CALL(cm, run())
		.Times(400)
		.WillRepeatedly(Return(true));

	EXPECT_CALL(cc, nandReadAndChecker(_,_))
		.Times(200)
		.WillRepeatedly(Return(false));

	ts.writeReadAging();

	// PASS string check
	EXPECT_EQ(oss.str(), "PASS");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, WriteReadAging_Test2) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// run fail check
	EXPECT_CALL(cm, set(_));
	EXPECT_CALL(cm, run())
		.WillRepeatedly(Return(false));

	ts.writeReadAging();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}

TEST(TS, WriteReadAging_Test3) {
	CommandMock cm;
	CheckerMock cc;
	TestScript ts{ &cm, &cc };

	std::ostringstream oss;
	std::streambuf* old_buf;

	old_buf = std::cout.rdbuf(oss.rdbuf());  // cout redirection

	// Command operation count check
	EXPECT_CALL(cm, set(_))
		.Times(2);
	EXPECT_CALL(cm, run())
		.Times(2)
		.WillRepeatedly(Return(true));

	EXPECT_CALL(cc, nandReadAndChecker(_,_))
		.WillRepeatedly(Return(true));

	ts.writeReadAging();

	// FAIL string check
	EXPECT_EQ(oss.str(), "FAIL");

	std::cout.rdbuf(old_buf);  // origin rdbuf  restore
}