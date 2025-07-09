#include "gmock/gmock.h"
#include "../runner.h"
#include "../full_write_and_read_compare_command.h"
#include "../partial_lba_write_command.h"
#include "../write_read_aging_command.h"
#include "../erase_and_write_aging_command.h"

#include <iostream>
#include <sstream>
#include <filesystem>

using namespace testing;

TEST(Runner, AllScriptSuccess) {
	SsdHandlerMock mockSsdHandler;
	UtilsMock mockUtils;
	Runner runner{ &mockSsdHandler, &mockUtils };

	EXPECT_CALL(mockSsdHandler, write(_, _))
		.Times(3590);
	EXPECT_CALL(mockSsdHandler, read(_))
		.Times(650);
	EXPECT_CALL(mockSsdHandler, erase(_, _))
		.Times(1471);
	EXPECT_CALL(mockUtils, genSSDRandData())
		.Times(3195);
	EXPECT_CALL(mockUtils, outputChecker(_))
		.Times(650)
		.WillRepeatedly(Return(true));

	std::filesystem::path path = std::filesystem::current_path();
	runner.runScriptFile(path.string() + "//shell_script.txt");
}



