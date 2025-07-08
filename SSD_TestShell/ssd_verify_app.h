#pragma once

#include <string>
#include "ssd_handler.h"

class SsdVerifyApp {
public:
	void startVerify(SsdInterface* sdInterface);
	void setInputCmd(std::string input);
	void getUserCmdLine();
private:
	std::string inputCmd;
};
