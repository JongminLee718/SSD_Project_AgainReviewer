#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include "logger.h"

using std::string;

void Logger::print(const string& methodName, const string& logMessage) {
	createLogFile();
	saveAsLogFile();
	writeToLogFile(createLogLine(methodName, logMessage));
}

string Logger::createLogLine(const string& methodName, const string& logMessage)
{
	string paddedMethod = padMethodName(methodName);
	return "[" + getCurrentTimestamp() + "] " +
		paddedMethod + " : " + logMessage + "\n";
}

void Logger::writeToLogFile(const string& logLine)
{
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
	return string(buf);
}

string Logger::padMethodName(const string& methodName, size_t width) {
	if (methodName.length() >= width) {
		return methodName;
	}
	return methodName + string(width - methodName.length(), ' ');
}

void Logger::saveAsLogFile() {
	string src = "latest.log";
	string dst = generateLogFilename();

	if (!std::filesystem::exists(src)) return;
	if (std::filesystem::file_size(src) < 10 * 1024) return;

	saveAsZipFile();

	std::filesystem::copy_file(src, dst,
		std::filesystem::copy_options::overwrite_existing);
	std::filesystem::remove(src);
}

void Logger::saveAsZipFile() {
	for (const auto& entry : std::filesystem::directory_iterator("logs")) {
		if (!entry.is_regular_file()) continue;

		const auto& path = entry.path();
		if (path.extension() == ".log" && path.filename() != "latest.log") {
			std::filesystem::path zipPath = path;
			zipPath.replace_extension(".zip" );

			std::filesystem::copy_file(path, zipPath,
				std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove(path);
		}
	}
}

string Logger::generateLogFilename() {
	std::time_t now = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &now);

	char buffer[64];
	std::snprintf(
		buffer, sizeof(buffer),
		"until_%02d%02d%02d_%02dh_%02dm_%02ds",
		(tm.tm_year % 100),
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec
	);

	return logDir +  std::string(buffer) + ".log";
}