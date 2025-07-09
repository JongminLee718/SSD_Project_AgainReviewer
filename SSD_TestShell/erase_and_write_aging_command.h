#pragma once

#include "command.h"
#include <iomanip>
#include <random>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ssd_handler.h"
#include "checker.h"

class EraseAndWriteAgingCommand : public Command {
public:
	EraseAndWriteAgingCommand(SsdInterface* ssd) : ssd{ ssd } {}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssd;
};