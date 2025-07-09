#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "logger.h"

using std::string;

void Logger::print(const string& methodName, const string& logMessage) {
	createLogFile();

	string paddedMethod = padMethodName(methodName);
	string logLine = "[" + getCurrentTimestamp() + "] " +
		paddedMethod + " : " + logMessage + "\n";

	std::ofstream out(logFile, std::ios::app);
	if (out.is_open()) {
		out << logLine;
	}
}

void Logger::createLogFile() {
	if (!std::filesystem::exists(logFile)) {
		std::ofstream out(logFile);
	}
}

string Logger::getCurrentTimestamp() {
	std::time_t now = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &now);

	char buf[20];
	std::strftime(buf, sizeof(buf), "%y.%m.%d %H:%M", &tm);
	return std::string(buf);
}

string Logger::padMethodName(const string& methodName, size_t width) {
	if (methodName.length() >= width) {
		return methodName;
	}
	return methodName + std::string(width - methodName.length(), ' ');
}