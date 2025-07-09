#include "utils.h"

Utils::Utils() : gen(std::random_device{}()), dist(0, 0xFFFFFFFF) {
}

bool Utils::outputChecker(const string& data) {
	std::ifstream ssdOutput("ssd_output.txt");
	string outputData;

	if (ssdOutput.is_open()) {
		if (!std::getline(ssdOutput, outputData)) return false;
	}
	ssdOutput.close();

	if (outputData != data) return false;
	return true;
}

string Utils::readOutput() {
    char curDir[1000];
    _getcwd(curDir, 1000);
    string filePath(curDir);
    filePath += "//ssd_output.txt";

    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "file open fail" << std::endl;
        return "";
    }

    std::string line = "";
    getline(file, line);

    file.close();

    return line;
}

string Utils::genRandData() {
    oss.str("");
    oss.clear();
    oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << dist(gen);
    return oss.str();
}