#pragma once
#include <string>
#include <sstream>
#include "ssd_handler.h"
#include "utils.h"

class Runner {
public:
    Runner(SsdInterface* ssd, UtilsInterface* utils) : ssdInterface{ ssd }, utilsInterface{ utils } {}
    void runScriptFile(const std::string& filePath);
    void executeAllTest(std::ifstream& fin);
    
private:
    bool isValidPath(std::ifstream& fin);
    bool executeOneTest(const std::string& cmd);
    void printCmd(const std::string& command);
    SsdInterface* ssdInterface;
    UtilsInterface* utilsInterface;
};