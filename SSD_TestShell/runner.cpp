#include "runner.h"
#include "ssd_client_app.h"
#include <iostream>
#include <iomanip>
#include <string>

using std::cout;
using std::string;

void Runner::runScriptFile(const string& filePath) {
    std::ifstream fin(filePath);
    if (!isValidPath(fin)) return;
    executeAllTest(fin);
}

void Runner::executeAllTest(std::ifstream& fin)
{
    string command;
    while (std::getline(fin, command)) {
        if (command.empty()) continue;

        printCmd(command);

        if (!executeOneTest(command)) {
            break;
        }
    }
}

void Runner::printCmd(std::string& command)
{
    cout << std::left << std::setw(30) << command << " ___   Run...";
}

bool Runner::isValidPath(std::ifstream& fin)
{
    if (!fin) {
        std::cerr << "Cannot open script file\n";
        return false;
    }
    return true;
}

bool Runner::executeOneTest(const string& cmd) {
    SsdHandler ssdHandler;
    Utils utils;
    SsdClientApp app;

    app.setInputCmd(cmd);
    return app.startVerify(&ssdHandler, &utils);
}
