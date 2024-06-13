#include <cstring>
#include <vector>
#include <fstream>
#include <windows.h>

#include <fileManager.cpp>

#include "opcodes.cpp"
#include "readRom.cpp"

Rom readBin(std::string filename) {
    Rom rom;
    std::ifstream file = openFile(filename);

    // Get file size
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    rom.resize(size);

    file.read(reinterpret_cast<char*>(rom.data()), rom.size());
    file.close();
    return rom;
}

void prgToAsm() { // takes prg and converts the binary into human readable assembly
    Rom prg = readBin("converted/binary/prg.bin");

    CreateDirectory("converted/assembly", NULL);
    std::ofstream file("converted/assembly/code.asm");

    uint8_t bytes = 0;
    for (int i = 0; i < prg.size(); i+=bytes) {
        const Opcode& op = opcodeTable[prg[i]];
        bytes = AddressingModeBytes[op.addressing]; // the amount of bytes the instruction takes up

        int number = 0;
        for (int j = 0; j < bytes-1; j++) { // read lo and hi bytes
            number += prg[i+1+j]<<(j*8);
        }

        file << OpcodeToAsm(op, number) << std::endl; // add instruction to the asm file    
    }

    file.close();
}

void asmToC() { // takes the converted assembly and turns it into c code
    CreateDirectory("converted/C", NULL);
}

void convertRom(const std::string& filename) {
    CreateDirectory("converted", NULL);

    RomToBin(filename);    

    // Convert to ASM 
    prgToAsm();

    // Convert to C

    //asmToC();
}