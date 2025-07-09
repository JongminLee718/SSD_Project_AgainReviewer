#include "gmock/gmock.h"
#include "../runner.h"
#include "../full_write_and_read_compare_command.h"
#include "../partial_lba_write_command.h"
#include "../write_read_aging_command.h"
#include "../erase_and_write_aging_command.h"

#include <iostream>
#include <sstream>

using namespace testing;

//TODO
TEST(Runner, AllScriptSuccess) {
	Runner runner;
	runner.runScriptFile("shell_script.txt");

	EXPECT_EQ(1, 1);
}



