#include <fstream>
#include "log_file_manager.h"

using std::string;

void LogFileManager::writeToLogFile(const string& logLine)
{
    std::ofstream out(logDir + logFile, std::ios::app);
    if (out.is_open()) {
        out << logLine;
    }
}

void LogFileManager::createLogFile() {
    if (!std::filesystem::exists(logDir + logFile)) {
        std::ofstream out(logDir + logFile);
    }
}