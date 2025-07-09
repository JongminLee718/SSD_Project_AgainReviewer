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

class PartialLBAWriteCommand : public Command {
public:
	PartialLBAWriteCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};