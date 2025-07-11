#pragma once

#include <string>

class LogArchiver {
public:
    LogArchiver(const std::string logDir, const std::string logFile) :logDir{ logDir }, logFile{ logFile } {}
    void archiveLogFile();
private:
    const std::string logDir;
    const std::string logFile;
    void saveAsZipFile();
    std::string generateLogFilename();
};