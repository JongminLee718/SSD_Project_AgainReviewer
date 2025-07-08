#pragma once

#include <string>

class SsdVerifyApp {
public:
	void startVerify();
	void setInputCmd(std::string input);
	void getUserCmdLine();
private:
	std::string inputCmd;
};
