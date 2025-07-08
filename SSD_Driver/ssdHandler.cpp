#include "ssdHandler.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

SSDHandler::SSDHandler(const std::string& nandFilePath) : filePath(nandFilePath) {
    initializeNandFile();
    loadNandData();
}

std::string SSDHandler::executeRead(int address) {
    if (address < 0 || address >= SSD_SIZE) {
        return "ERROR";
    }

    unsigned int readValue = nandData[address];

    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
    return ss.str();
}

void SSDHandler::initializeNandFile() {
    std::ifstream file(filePath);
    if (file.good()) {
        return;
    }
    std::ofstream newFile(filePath);
    if (!newFile.is_open()) {
        throw std::runtime_error("Failed to create NAND file.");
    }
    for (int i = 0; i < SSD_SIZE; ++i) {
        newFile << "0x00000000\n";
    }
}

void SSDHandler::loadNandData() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open NAND file for reading.");
    }
    nandData.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            nandData.push_back(std::stoul(line, nullptr, 16));
        }
    }
}