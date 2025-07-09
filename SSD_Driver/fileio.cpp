#include <fstream>
#include <iostream>
#include "fileio.h"
#include "main.h"

FileInOut::FileInOut(const std::string& nandFilePath) : filePath(nandFilePath) {
    initializeNandFile();
    loadNandData();
}

void FileInOut::initializeNandFile() {
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

void FileInOut::loadNandData() {
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