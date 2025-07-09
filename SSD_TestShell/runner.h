#pragma once
#include <string>
#include <sstream>

class Runner {
public:
    void runScriptFile(const std::string& filePath);
    void executeAllTest(std::ifstream& fin);
    void printCmd(std::string& command);
private:
    bool isValidPath(std::ifstream& fin);
    bool executeOneTest(const std::string& cmd);
};