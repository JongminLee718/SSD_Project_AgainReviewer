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

class WriteReadAgingCommand : public Command {
public:
	WriteReadAgingCommand(SsdInterface* ssd, Checker* cc) : ssd{ ssd }, cc{ cc } {}
	void run(vector<string> commands) override;
private:
	SsdInterface* ssd;
	Checker* cc;
};