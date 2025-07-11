#pragma once

#include <string>

#include "log_file_manager.h"
#include "log_formatter.h"
#include "log_archiver.h"

#if defined(__GNUC__) || defined(__clang__)
#define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define CURRENT_FUNCTION __FUNCSIG__
#else
#define CURRENT_FUNCTION __func__
#endif

#define LOG(msg) Logger::getInstance().print(msg, CURRENT_FUNCTION)

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void print(const std::string& logMessage, const std::string& callerInfo);

private:
	Logger() :
		fileManager{ logDir, logFile },
		archiver{ logDir, logFile } {}
	~Logger() = default;
	const std::string logDir = "logs/";
	const std::string logFile = "latest.log";

	LogFormatter formatter;
	LogFileManager fileManager;
	LogArchiver archiver;
};