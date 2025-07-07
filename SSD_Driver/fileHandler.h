#pragma once
#include <vector>
#include <string>

class FileHandler {
public:
	void initializeNand();
	void initializeOutput();
	std::vector<std::string> readNandData();
	void writeOutput(const std::string& value);
};