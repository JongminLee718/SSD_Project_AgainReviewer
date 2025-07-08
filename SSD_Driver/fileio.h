#pragma once
#include <string>
#include <vector>

class FileInOut {
public:
    std::vector<unsigned int> nandData;
    FileInOut(const std::string& nandFilePath);

private:
    
    void initializeNandFile();
    void loadNandData();

    std::string filePath;

    const int SSD_SIZE = 100;
};