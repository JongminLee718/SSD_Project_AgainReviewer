#include "command_mock.h"
#include "shell_command.h"
#include "test_script.cpp"

using namespace testing;

TEST(TS, TC1) {
	CommandMock cm;
	TestScript ts{ &cm };

	EXPECT_CALL(cm, run())
		.Times(200);
		
	EXPECT_CALL(cm, set(_))
		.Times(200);

	ts.fullWriteAndReadCompare();
}

TEST(TS, TC2) {
	CommandMock cm;
	TestScript ts{ &cm };

	EXPECT_CALL(cm, run())
		.Times(120);

	EXPECT_CALL(cm, set(_))
		.Times(120);

	ts.partialLBAWrite();
}

TEST(TS, TC3) {
	CommandMock cm;
	TestScript ts{ &cm };

	EXPECT_CALL(cm, run())
		.Times(400);

	EXPECT_CALL(cm, set(_))
		.Times(400);

	ts.writeReadAging();
}
