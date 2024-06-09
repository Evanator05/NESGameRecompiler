#include <cstring>
#include <vector>
#include <fstream>
#include <windows.h>

struct Header {
    char magic[4]; // Identifier or magic number. This should be "NES" followed by the MS-DOS end-of-file character (1A hex).
    uint8_t prgSize; // Size of PRG ROM in 16 KB units. This indicates the size of the program code. prgSize*16000 to get value
    uint8_t chrSize; // Size of CHR ROM in 8 KB units. This indicates the size of the graphics data. chrSize*8000 to get value
    char flags6; // Flags 6. This byte contains information about various attributes of the ROM, such as mirroring type and battery-backed PRG RAM.
    char flags7; // Flags 7. This byte also contains information about the ROM, such as whether it has a trainer or uses four-screen VRAM.
    char padding[8]; // These bytes are not used by the NES itself and can be used for various purposes by ROM hackers or developers.
};

// used for converting prg into asm

enum addressingMode {
    A,
    ABS,
    ABSX,
    ABSY,
    IMD,
    IMP,
    IND,
    XIND,
    INDY,
    REL,
    ZPG,
    ZPGX,
    ZPGY
};

struct opCode {
    std::string name;
    addressingMode addressing;
};

std::string instructionTable[] = { 
                                "BRK", "ORA", "???", "???", "???", "ORA", "ASL", "???", "PHP", "ORA", "ASL", "???", "???", "ORA", "ASL", "???",
                                "BPL", "ORA", "???", "???", "???", "ORA", "ASL", "???", "CLC", "ORA", "???", "???", "???", "ORA", "ASL", "???",
                                "JSR", "AND", "???", "???", "BIT", "AND", "ROL", "???", "PLP", "AND", "ROL", "???", "BIT", "AND", "ROL", "???",
                                "BMI", "AND", "???", "???", "???", "AND", "ROL", "???", "SEC", "AND", "???", "???", "???", "AND", "ROL", "???",
                                "RTI", "EOR", "???", "???", "???", "EOR", "LSR", "???", "PHA", "EOR", "LSR", "???", "JMP", "EOR", "LSR", "???",
                                "BVC", "EOR", "???", "???", "???", "EOR", "LSR", "???", "CLI", "EOR", "???", "???", "???", "EOR", "LSR", "???",
                                "RTS", "ADC", "???", "???", "???", "ADC", "ROR", "???", "PLA", "ADC", "ROR", "???", "JMP", "ADC", "ROR", "???",
                                "BVS", "ADC", "???", "???", "???", "ADC", "ROR", "???", "SEI", "ADC", "???", "???", "???", "ADC", "ROR", "???",
                                "???", "STA", "???", "???", "STY", "STA", "STX", "???", "DEY", "???", "TXA", "???", "STY", "STA", "STX", "???",
                                "BCC", "STA", "???", "???", "STY", "STA", "STX", "???", "TYA", "STA", "TXS", "???", "???", "STA", "???", "???",
                                "LDY", "LDA", "LDX", "???", "LDY", "LDA", "LDX", "???", "TAY", "LDA", "TAX", "???", "LDY", "LDA", "LDX", "???",
                                "BCS", "LDA", "???", "???", "LDY", "LDA", "LDX", "???", "CLV", "LDA", "TSX", "???", "LDY", "LDA", "LDX", "???",
                                "CPY", "CMP", "???", "???", "CPY", "CMP", "DEC", "???", "INY", "CMP", "DEX", "???", "CPY", "CMP", "DEC", "???",
                                "BNE", "CMP", "???", "???", "???", "CMP", "DEC", "???", "CLD", "CMP", "???", "???", "???", "CMP", "DEC", "???",
                                "CPX", "SBC", "???", "???", "CPX", "SBC", "INC", "???", "INX", "SBC", "NOP", "???", "CPX", "SBC", "INC", "???",
                                "BEQ", "SBC", "???", "???", "???", "SBC", "INC", "???", "SED", "SBC", "???", "???", "???", "SBC", "INC", "???"
                                };

typedef std::vector<uint8_t> GameRom;

std::ifstream openFile(const std::string &filePath) {
    // Open file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    // Check if file opened successfully
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    
    return file;
}

void createFile(const std::string& filename, GameRom& data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void readHeader(std::ifstream &file, Header &header) { // reads information from the header such as its identifier prg size and chr size
    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
}

void readRom(std::ifstream &file, Header &header, GameRom& prg, GameRom& chr) { // takes the rom and breaks it into prg and chr
    file.read(reinterpret_cast<char*>(prg.data()), prg.size());
    file.read(reinterpret_cast<char*>(chr.data()), chr.size());
}

void prgToAsm(GameRom &rom) { // takes prg and converts the binary into human readable assembly

}

void asmToC() { // takes the converted assembly and turns it into c code

}



void convertRom(const std::string& filename) {
    std::ifstream file = openFile(filename); 
    
    Header header;
    readHeader(file, header);

    GameRom prg(header.prgSize* 16 * 1024);
    GameRom chr(header.chrSize* 8 * 1024);
    readRom(file, header, prg, chr);
    
    //for (int i = 0; i < sizeof(prg)/3; i++) {
    //    std::cout.setf(std::ios::hex | std::ios::uppercase | std::ios::showbase);
    //    std::cout << prg[i*3] << std::endl;
    //}

    //prgToAsm(prg);

    //asmToC();

    CreateDirectory("binary", NULL);

    createFile("binary/prg.bin", prg);
    createFile("binary/chr.chr", chr);

    
    file.close();
}