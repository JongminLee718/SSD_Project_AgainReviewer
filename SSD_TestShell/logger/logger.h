#include <string>

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void print(const std::string& methodName, const std::string& logMessage);
	std::string getLogFile() { return logFile; }

private:
	Logger() = default;
	~Logger() = default;

	const std::string logFile = "latest.log";
	void createLogFile();
	std::string getCurrentTimestamp();
	std::string padMethodName(const std::string& methodName, size_t width = 50);
	void writeToLogFile(const std::string& logLine);
	std::string createLogLine(const std::string& methodName, const std::string& logMessage);
	void saveAsLogFile();
	void saveAsZipFile();
	std::string generateLogFilename();
};