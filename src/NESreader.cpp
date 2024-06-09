#include <cstring>
#include <vector>
#include <fstream>
#include <windows.h>

#include "opcodes.cpp"

struct Header {
    char magic[4]; // Identifier or magic number. This should be "NES" followed by the MS-DOS end-of-file character (1A hex).
    uint8_t prgSize; // Size of PRG ROM in 16 KB units. This indicates the size of the program code. prgSize*16000 to get value
    uint8_t chrSize; // Size of CHR ROM in 8 KB units. This indicates the size of the graphics data. chrSize*8000 to get value
    char flags6; // Flags 6. This byte contains information about various attributes of the ROM, such as mirroring type and battery-backed PRG RAM.
    char flags7; // Flags 7. This byte also contains information about the ROM, such as whether it has a trainer or uses four-screen VRAM.
    char padding[8]; // These bytes are not used by the NES itself and can be used for various purposes by ROM hackers or developers.
};

typedef std::vector<uint8_t> Rom;

std::ifstream openFile(const std::string &filePath) {
    // Open file in binary mode
    std::ifstream file(filePath, std::ios::binary);
    
    // Check if file opened successfully
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    
    return file;
}

void createFile(const std::string& filename, Rom &data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void readHeader(std::ifstream &file, Header &header) { // reads information from the header such as its identifier prg size and chr size
    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
}

void readRom(std::ifstream &file, Header &header, Rom &prg, Rom &chr) { // takes the rom and breaks it into prg and chr
    file.read(reinterpret_cast<char*>(prg.data()), prg.size());
    file.read(reinterpret_cast<char*>(chr.data()), chr.size());
}

void prgToAsm(Rom &prg) { // takes prg and converts the binary into human readable assembly
    CreateDirectory("assembly", NULL);
    std::ofstream file("assembly/code.asm");

    int i = 0;
    while (i < prg.size()) {
        const Opcode& op = opcodeTable[prg[i]];

        int number = 0;
        for (int j = 0; j < op.bytes-1; j++) {
            int n = prg[i+1+j];
            n = n<<(j*8);
            number += n;
        }

        file << OpcodeToAsm(op, number) << std::endl;
                      
        i += op.bytes;
    }

    file.close();
}

void asmToC() { // takes the converted assembly and turns it into c code

}

void convertRom(const std::string& filename) {
    // Extract Binary From Rom
    std::ifstream file = openFile(filename); 
    
    Header header;
    readHeader(file, header);

    Rom prg(header.prgSize * 16 * 1024);
    Rom chr(header.chrSize * 8 * 1024);
    readRom(file, header, prg, chr);

    file.close();

    CreateDirectory("binary", NULL);

    createFile("binary/prg.bin", prg);
    createFile("binary/chr.chr", chr);

    // Convert to ASM 
    prgToAsm(prg);

    //asmToC();
}