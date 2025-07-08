#include "ssd_handler.h"
#include "string"
#include "iostream"
#include <fstream>
#include <direct.h> 

using std::string;
using std::getline;

void SsdHandler::read(string lba) {
    string cmd = "//SSD_Driver.exe R " + lba ;
    runSSD(cmd);
}

void SsdHandler::write(string lba, string data) {
    string cmd = "//SSD_Driver.exe W " + lba + " " + data;
    runSSD(cmd);
}

string SsdHandler::readOutput() {
    char curDir[1000];
    _getcwd(curDir, 1000);
    string filePath(curDir);
    filePath += "//ssd_output.txt";

    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "file open fail" << std::endl;
        return "";
    }

    std::string line;
    getline(file, line);

    file.close();

    return line;
}

void SsdHandler::runSSD(std::string cmd)
{
    char curDir[1000];
    _getcwd(curDir, 1000);

    string exePath(curDir);
    exePath += cmd;

    system(exePath.c_str());
}
