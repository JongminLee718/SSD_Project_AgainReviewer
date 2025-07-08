#include <string>
#include <vector>

class SSDHandler {
public:
    explicit SSDHandler(const std::string& nandFilePath);
    std::string executeRead(int lba);

private:
    void initializeNandFile();
    void loadNandData();

    std::string filePath;
    std::vector<unsigned int> nandData;
    const int SSD_SIZE = 100;
};
