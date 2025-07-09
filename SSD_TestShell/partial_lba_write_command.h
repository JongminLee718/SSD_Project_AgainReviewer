#pragma once

#include "command.h"
#include "ssd_handler.h"
#include "utils.h"

class PartialLBAWriteCommand : public Command {
public:
	PartialLBAWriteCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};