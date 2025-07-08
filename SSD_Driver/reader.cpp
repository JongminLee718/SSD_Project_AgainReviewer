#include "reader.h"
#include <sstream>
#include <iomanip>

ssdProgram::ssdProgram(InterfaceStorage* storage) : storageDevice(storage) {}

std::string ssdProgram::executeRead(int lba) {
    unsigned int readValue;
    if (storageDevice->read(lba, readValue)) {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << readValue;
        return ss.str();
    }
    return "ERROR";
}