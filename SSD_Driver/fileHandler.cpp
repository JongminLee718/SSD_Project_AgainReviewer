#include "fileHandler.h"
#include <iomanip>
#include <iostream>
#include <fstream>

static const int NAND_SIZE = 100;
const std::string OUTPUT_FILE = "ssd_output.txt";
const std::string NAND_FILE = "ssd_nand.txt";

void FileHandler::initializeNand() {
    std::ifstream ifs(NAND_FILE);
    if (!ifs.is_open()) {
        std::ofstream ofs(NAND_FILE);
        if (ofs.is_open()) {
            for (int i = 0; i < NAND_SIZE; ++i) {
                ofs << "0x00000000\n";
            }
        }
    }
}

void FileHandler::initializeOutput() {
    std::ofstream ofs(OUTPUT_FILE, std::ios::trunc);
}

std::vector<std::string> FileHandler::readNandData() {
    initializeNand();
    std::vector<std::string> data;
    std::ifstream ifs(NAND_FILE);
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            data.push_back(line);
        }
    }
    return data;
}

void FileHandler::writeOutput(const std::string& value) {
    std::ofstream ofs(OUTPUT_FILE);
    if (ofs.is_open()) {
        ofs << value;
    }
}