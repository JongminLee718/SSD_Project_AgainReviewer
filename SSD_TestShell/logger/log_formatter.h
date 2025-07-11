#pragma once

#include <string>

class LogFormatter {
public:
    std::string createLogLine(const std::string& logMessage, const std::string& callerInfo);
private:
    std::string getCurrentTimestamp();
    std::string parseMethodName(const std::string& callerInfo);
    std::string padMethodName(const std::string& methodName, size_t width);
};