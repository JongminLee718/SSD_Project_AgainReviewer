#include "ssd_handler.h"
#include "string"
#include <direct.h> 

using std::string;

void SsdHandler::read(string lba) {
    string cmd = "SSD.exe R " + lba ;
    runSSD(cmd);
}

void SsdHandler::write(string lba, string data) {
    string cmd = "SSD.exe W " + lba + " " + data;
    runSSD(cmd);
}

void SsdHandler::runSSD(std::string cmd)
{
    char curDir[1000];
    _getcwd(curDir, 1000);

    string exePath(curDir);
    exePath += cmd;

    system(exePath.c_str());
}
