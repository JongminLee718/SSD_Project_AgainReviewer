#pragma once

#include <string>
#include <filesystem>

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
	std::string getLogFile() { return logFile; }

private:
	Logger() {
		std::filesystem::create_directories(logDir);
	}
	~Logger() = default;

	const std::string logDir = "logs/";
	const std::string logFile = logDir + "latest.log";
	void createLogFile();
	std::string getCurrentTimestamp();
	std::string parseMethodName(const std::string& prettyFunc);
	std::string padMethodName(const std::string& methodName, size_t width = 50);
	void writeToLogFile(const std::string& logLine);
	std::string createLogLine(const std::string& methodName, const std::string& logMessage);
	void saveAsLogFile();
	void saveAsZipFile();
	std::string generateLogFilename();
};