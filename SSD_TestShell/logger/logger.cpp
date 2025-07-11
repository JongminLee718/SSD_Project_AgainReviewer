#include "logger.h"

using std::string;

void Logger::print(const string& logMessage, const std::string& callerInfo) {
	archiver.archiveLogFile();
	fileManager.writeToLogFile(formatter.createLogLine(logMessage, callerInfo));
}