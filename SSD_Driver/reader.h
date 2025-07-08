#include <string>
#include <vector>

class SSDHandler {
public:
    std::vector<unsigned int>& nandData;
    SSDHandler(std::vector<unsigned int>& data) : nandData(data) {}
    std::string executeRead(int lba);

private:
    const int SSD_SIZE = 100;
};
