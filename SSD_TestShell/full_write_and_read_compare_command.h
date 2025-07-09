#pragma once

#include "command.h"
#include <iomanip>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ssd_handler.h"
#include "utils.h"

class FullWriteAndReadCompareCommand : public Command {
public:
	FullWriteAndReadCompareCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};