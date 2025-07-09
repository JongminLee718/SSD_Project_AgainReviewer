#pragma once
#include <string>
#include <iostream>
#include <sstream>

class Runner {
public:
    void runScriptFile(const std::string& filePath);
private:
    bool executeTestScript(const std::string& cmd);
};