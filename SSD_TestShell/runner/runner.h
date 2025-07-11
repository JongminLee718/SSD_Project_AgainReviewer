#pragma once
#include <string>
#include <sstream>
#include "utils.h"
#include "script_strategy_interface.h"

class Runner {
public:
    Runner(std::unique_ptr<ScriptStrategyInterface> strategy)
        : testStrategy(std::move(strategy)) {
    }

    void runScriptFile(const std::string& filePath);
    void executeAllTest(std::ifstream& fin);

private:
    bool isValidPath(std::ifstream& fin);
    bool executeOneTest(const std::string& cmd);
    void printCmd(const std::string& command);

    std::unique_ptr<ScriptStrategyInterface> testStrategy;
};