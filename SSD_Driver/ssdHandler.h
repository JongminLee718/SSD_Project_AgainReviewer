#include <string>
#include <vector>

class SSDHandler {
public:
    std::vector<unsigned int> nandData;
    explicit SSDHandler(const std::string& nandFilePath);
    std::string executeRead(int lba);

private:
    void initializeNandFile();
    void loadNandData();

    std::string filePath;

    const int SSD_SIZE = 100;
};
