#include "reader.h"
#include "fileio.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string SSDHandler::executeRead(int address) {
    if (address < 0 || address >= SSD_SIZE) {
        return "ERROR";
    }
    unsigned int readValue = nandData[address];
    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << readValue;
    return ss.str();
}

