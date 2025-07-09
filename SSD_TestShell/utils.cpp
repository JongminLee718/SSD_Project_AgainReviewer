#include "utils.h"

Utils::Utils() : gen(std::random_device{}()), dist(0, 0xFFFFFFFF) {}

bool Utils::outputChecker(const string& data) {
    string outputData = this->readOutput();
	if (outputData != data) return false;
	return true;
}

string Utils::readOutput() {
    std::ifstream ssdOutput("ssd_output.txt");
    string outputData;

    if (!ssdOutput.is_open()) {
        std::cerr << "file open fail" << std::endl;
        return "";
    }

    if (!std::getline(ssdOutput, outputData)) return "";
    ssdOutput.close();

    return outputData;
}

string Utils::genSSDRandData() {
    oss.str("");
    oss.clear();
    oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << dist(gen);
    return oss.str();
}