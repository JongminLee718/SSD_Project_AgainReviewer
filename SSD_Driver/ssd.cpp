#include "ssd.h"
#include "fileHandler.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

static const int NAND_SIZE = 100;
const std::string OUTPUT_FILE = "ssd_output.txt";
const std::string NAND_FILE = "ssd_nand.txt";
const std::string ERROR_MESSAGE = "ERROR";

void SSD::doWriteCmd() {
	return;
}

void SSD::doReadCmd(const std::string& command_type, int lba) {
    FileHandler fileHandler;
    fileHandler.initializeOutput();
	std::vector<std::string> nand_data = fileHandler.readNandData();

    if (command_type == "R") {
        if (lba < 0 || lba >= NAND_SIZE) {
            fileHandler.writeOutput(ERROR_MESSAGE);
            return;
        }
        fileHandler.writeOutput(nand_data[lba]);
    }
	return;
}