#include <string>

class Logger {
public:
	Logger() = default;
	~Logger() = default;

	void print(const std::string& methodName, const std::string& logMessage);
	std::string getLogFile() { return logFile; }

private:
	const std::string logFile = "latest.log";
	void createLogFile();
	std::string getCurrentTimestamp();
	std::string padMethodName(const std::string& methodName, size_t width = 50);
	void writeToLogFile(const std::string& logLine);
	std::string createLogLine(const std::string& methodName, const std::string& logMessage);
};