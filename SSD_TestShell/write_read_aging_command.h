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

class WriteReadAgingCommand : public Command {
public:
	WriteReadAgingCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};