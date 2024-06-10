#include <cstring>
#include <vector>
#include <fstream>
#include <windows.h>

#include <fileManager.cpp>

#include "opcodes.cpp"
#include "readRom.cpp"

Rom ReadBin(std::string filename) {
    Rom rom;
    std::ifstream file = openFile(filename);
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    rom.resize(size);
    file.read(reinterpret_cast<char*>(rom.data()), rom.size());
    file.close();
    return rom;
}

void prgToAsm() { // takes prg and converts the binary into human readable assembly
    Rom prg = ReadBin("converted/binary/prg.bin");

    CreateDirectory("converted/assembly", NULL);
    std::ofstream file("converted/assembly/code.asm");

    int i = 0;
    while (i < prg.size()) {
        const Opcode& op = opcodeTable[prg[i]];
        const uint8_t& bytes = AddressingModeBytes[op.addressing]; // the amount of bytes the instruction takes up

        int number = 0;
        for (int j = 0; j < bytes-1; j++) { // read lo and hi bits
            int n = prg[i+1+j];
            n = n<<(j*8);
            number += n;
        }

        file << OpcodeToAsm(op, number) << std::endl; // add instruction to the asm file
                      
        i += bytes;
    }

    file.close();
}

void asmToC() { // takes the converted assembly and turns it into c code

}

void convertRom(const std::string& filename) {
    CreateDirectory("converted", NULL);

    RomToBin(filename);    

    // Convert to ASM 
    prgToAsm();

    //asmToC();
}