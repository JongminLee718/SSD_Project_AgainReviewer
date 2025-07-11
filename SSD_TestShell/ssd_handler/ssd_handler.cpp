#include "ssd_handler.h"
#include "string"
#include "iostream"
#include <fstream>
#include <direct.h> 

using std::string;
using std::getline;

void SsdHandler::read(string lba) {
    string cmd = "//SSD_Driver.exe R " + lba ;
    excute(cmd);
}

void SsdHandler::write(string lba, string data) {
    string cmd = "//SSD_Driver.exe W " + lba + " " + data;
    excute(cmd);
}

void SsdHandler::excute(std::string cmd)
{
    char curDir[1000];
    _getcwd(curDir, 1000);

    string exePath(curDir);
    exePath += cmd;

    system(exePath.c_str());
}

void SsdHandler::erase(string lba, string size) {
    string cmd = "//SSD_Driver.exe E " + lba + " " + size;
    excute(cmd);
}

void SsdHandler::flush() {
    string cmd = "//SSD_Driver.exe F ";
    excute(cmd);
}