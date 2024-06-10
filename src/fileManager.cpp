#include <iostream>
#include <fstream>
#include <vector>

std::ifstream openFile(const std::string &filePath) {
    // Open file in binary mode
    std::ifstream file(filePath, std::ios::binary);
    
    // Check if file opened successfully
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    
    return file;
}

void createFile(const std::string& filename, std::vector<uint8_t> &data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
}