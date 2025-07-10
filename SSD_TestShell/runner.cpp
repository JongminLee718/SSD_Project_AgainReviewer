#include "runner.h"
#include "ssd_client_app.h"
#include "logger.h"
#include <iostream>
#include <iomanip>
#include <string>

using std::cout;
using std::string;

void Runner::runScriptFile(const string& filePath) {
    LOG("run shell_script.txt file");
    std::ifstream fin(filePath);
    if (!isValidPath(fin)) return;
    LOG("valid file path");
    executeAllTest(fin);
}

void Runner::executeAllTest(std::ifstream& fin)
{
    LOG("execute all test");
    string command;
    while (std::getline(fin, command)) {
        if (command.empty()) continue;

        printCmd(command);

        if (!executeOneTest(command)) {
            LOG("FAIL - execute test");
            break;
        }
        LOG("SUCCESS - execute test");
    }
}

void Runner::printCmd(const string& command)
{
    std::ostringstream oss;
    oss << std::setfill(' ') << std::left << std::setw(27) << command << " ___   Run...";
    cout << oss.str();
}

bool Runner::isValidPath(std::ifstream& fin)
{
    if (!fin) {
        LOG("FAIL - nvalid file path");
        return false;
    }
    return true;
}

bool Runner::executeOneTest(const string& cmd) {
    if (!testStrategy) {
        LOG("FAIL - No Test");
        return false;
    }
    return testStrategy->execute(cmd);
}
