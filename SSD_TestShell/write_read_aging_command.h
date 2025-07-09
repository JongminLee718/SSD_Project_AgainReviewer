#pragma once

#include "command.h"
#include "ssd_handler.h"
#include "utils.h"

class WriteReadAgingCommand : public Command {
public:
	WriteReadAgingCommand(SsdInterface* ssd, UtilsInterface* utils) : ssd{ ssd }, utils{ utils } {}
	bool run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	UtilsInterface* utils;
};