#include "runner.h"
#include "ssd_client_app.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

using std::cout;
using std::vector;
using std::string;

void Runner::runScriptFile(const string& filePath) {
    std::ifstream fin(filePath);
    if (!fin) {
        std::cerr << "Cannot open script file: " << filePath << "\n";
        return;
    }

    string command;
    while (std::getline(fin, command)) {
        if (command.empty()) continue;

        std::cout << std::left << std::setw(30) << command << " ___   Run...";

        bool isPass = executeTestScript(command);

        if (!isPass) {
            break;
        }
    }
}

bool Runner::executeTestScript(const string& cmd) {
    SsdHandler ssdHandler;
    Utils utils;
    SsdClientApp app;

    app.setInputCmd(cmd);
    return app.startVerify(&ssdHandler, &utils);
}
