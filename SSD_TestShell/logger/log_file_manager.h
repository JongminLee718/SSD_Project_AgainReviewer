#pragma once

#include <string>
#include <filesystem>

class LogFileManager {
public:
    LogFileManager(const std::string logDir, const std::string logFile) :logDir{ logDir }, logFile{ logFile } {
        std::filesystem::create_directories(logDir);
    }
    void writeToLogFile(const std::string& logLine);
private:
    const std::string logDir;
    const std::string logFile;

    void createLogFile();
};
