#include <iostream>
#include "NESReader.cpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "No Rom Specified" << std::endl;
        return 1;
    }
    
    std::string romPath = argv[1];

    convertRom(romPath);
    
    std::cout << "Converted Rom" << std::endl;
    return 0;
}
