#include "InterfaceStorage.h"
#include <string>

class ssdProgram {
public:
    explicit ssdProgram(InterfaceStorage* storage);
    std::string executeRead(int lba);

private:
    InterfaceStorage* storageDevice;
};
