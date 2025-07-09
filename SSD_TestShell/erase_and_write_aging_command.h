#pragma once

#include "command.h"
#include "ssd_handler.h"
#include "utils.h"

class EraseAndWriteAgingCommand : public Command {
public:
	EraseAndWriteAgingCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};