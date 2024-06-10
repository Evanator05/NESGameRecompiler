#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower


enum AddressingMode {
    ACC,    // Accumulator
    ABS,  // Absolute
    ABX,  // Absolute, X-indexed
    ABY,  // Absolute, Y-indexed
    IMM,  // Immediate
    IMP,  // Implied
    IND,  // Indirect
    IZX,  // X-indexed, indirect 
    IZY, // Indirect, Y-indexed
    REL,  // Relative
    ZPG,  // Zeropage
    ZPX,  // Zeropage, X-indexed
    ZPY   // Zeropage, Y-indexed
};

enum Instructions {
    ADC, // Add with carry
    AND, // And (with accumulator)
    ASL, // Arithmetic shift left
    BCC, // Branch on carry clear
    BCS, // Branch on carry set
    BEQ, // Branch on equal (zero set)
    BIT, // Bit test
    BMI, // Branch on minus (negative set)
    BNE, // Branch on not equal (zero clear)
    BPL, // Branch on plus (negative clear)
    BRK, // Break / Interrupt
    BVC, // Branch on overflow clear
    BVS, // Branch on overflow set
    CLC, // Carry clear
    CLD, // Clear decimal
    CLI, // Clear interrupt disable
    CLV, // Clear overflow
    CMP, // Compare (with accumulator)
    CPX, // Compare with X
    CPY, // Compare with Y
    DEC, // Decrement
    DEX, // Decrement X
    DEY, // Decrement Y
    EOR, // Exclusive or (with accumulator)
    INC, // Increment
    INX, // Increment X
    INY, // Increment Y
    JMP, // Jump
    JSR, // Jump subroutine
    LDA, // Load accumulator
    LDX, // Load X
    LDY, // Load Y
    LSR, // Logical shift right
    NOP, // No operation
    ORA, // Or (with accumulator)
    PHA, // Push Accumulator
    PHP, // Push processor status (SR)
    PLA, // Pull accumulator
    PLP, // Pull prosessor status (SR)
    ROL, // Rotate left
    ROR, // Rotate right
    RTI, // Return from interrupt
    RTS, // Return from subroutine
    SBC, // Subtract with carry
    SEC, // Set carry
    SED, // Set decimal
    SEI, // Set interrupt disable
    STA, // Store accumulator
    STX, // Store X
    STY, // Store Y
    TAX, // Transfer accumulator to X
    TAY, // Transfer accumulator to Y
    TSX, // Transfer stack pointer to X
    TXA, // Transfer X to accumulator
    TXS, // Transfer X to stack pointer
    TYA, // Transfer Y to accumulator
    XXX  // Illegal Instruction
};
const char* InstructionStrings[] = {
    "ADC", // Add with carry
    "AND", // And (with accumulator)
    "ASL", // Arithmetic shift left
    "BCC", // Branch on carry clear
    "BCS", // Branch on carry set
    "BEQ", // Branch on equal (zero set)
    "BIT", // Bit test
    "BMI", // Branch on minus (negative set)
    "BNE", // Branch on not equal (zero clear)
    "BPL", // Branch on plus (negative clear)
    "BRK", // Break / Interrupt
    "BVC", // Branch on overflow clear
    "BVS", // Branch on overflow set
    "CLC", // Carry clear
    "CLD", // Clear decimal
    "CLI", // Clear interrupt disable
    "CLV", // Clear overflow
    "CMP", // Compare (with accumulator)
    "CPX", // Compare with X
    "CPY", // Compare with Y
    "DEC", // Decrement
    "DEX", // Decrement X
    "DEY", // Decrement Y
    "EOR", // Exclusive or (with accumulator)
    "INC", // Increment
    "INX", // Increment X
    "INY", // Increment Y
    "JMP", // Jump
    "JSR", // Jump subroutine
    "LDA", // Load accumulator
    "LDX", // Load X
    "LDY", // Load Y
    "LSR", // Logical shift right
    "NOP", // No operation
    "ORA", // Or (with accumulator)
    "PHA", // Push Accumulator
    "PHP", // Push processor status (SR)
    "PLA", // Pull accumulator
    "PLP", // Pull processor status (SR)
    "ROL", // Rotate left
    "ROR", // Rotate right
    "RTI", // Return from interrupt
    "RTS", // Return from subroutine
    "SBC", // Subtract with carry
    "SEC", // Set carry
    "SED", // Set decimal
    "SEI", // Set interrupt disable
    "STA", // Store accumulator
    "STX", // Store X
    "STY", // Store Y
    "TAX", // Transfer accumulator to X
    "TAY", // Transfer accumulator to Y
    "TSX", // Transfer stack pointer to X
    "TXA", // Transfer X to accumulator
    "TXS", // Transfer X to stack pointer
    "TYA", // Transfer Y to accumulator
    "XXX"  // Illegal Instruction
};

struct Opcode {
    Instructions name;
    AddressingMode addressing;
    uint8_t bytes;
};
Opcode __illegal__ = {XXX, IMP, 1};

Opcode opcodeTable[] =
{// 0           1           2           3           4           5           6           7           8           9           A           B           C           D           E           F
    {BRK,IMP,1},{ORA,IZX,2},__illegal__,__illegal__,__illegal__,{ORA,ZPG,2},{ASL,ZPG,2},__illegal__,{PHP,IMP,1},{ORA,IMM,2},{ASL,ACC,1},__illegal__,__illegal__,{ORA,ABS,3},{ASL,ABS,3},__illegal__, // 0
    {BPL,REL,2},{ORA,IZY,2},__illegal__,__illegal__,__illegal__,{ORA,ZPX,2},{ASL,ZPX,2},__illegal__,{CLC,IMP,1},{ORA,ABY,3},__illegal__,__illegal__,__illegal__,{ORA,ABS,3},{ASL,ABX,3},__illegal__, // 1
    {JSR,ABS,3},{AND,IZX,2},__illegal__,__illegal__,{BIT,ZPG,2},{AND,ZPG,2},{ROL,ZPG,2},__illegal__,{PLP,IMP,1},{AND,IMM,2},{ROL,ACC,1},__illegal__,{BIT,ABS,3},{AND,ABS,3},{ROL,ABS,3},__illegal__, // 2
    {BMI,REL,2},{AND,IZY,2},__illegal__,__illegal__,__illegal__,{AND,ZPX,2},{ROL,ZPX,2},__illegal__,{SEC,IMP,1},{AND,ABY,3},__illegal__,__illegal__,__illegal__,{AND,ABS,3},{ROL,ABX,3},__illegal__, // 3
    {RTI,IMP,1},{EOR,IZX,2},__illegal__,__illegal__,__illegal__,{EOR,ZPG,2},{LSR,ZPG,2},__illegal__,{PHA,IMP,1},{EOR,IMM,2},{LSR,ACC,1},__illegal__,{JMP,ABS,3},{EOR,ABS,3},{LSR,ABS,3},__illegal__, // 4
    {BVC,REL,2},{EOR,IZY,2},__illegal__,__illegal__,__illegal__,{EOR,ZPX,2},{LSR,ZPX,2},__illegal__,{CLI,IMP,1},{EOR,ABY,3},__illegal__,__illegal__,__illegal__,{EOR,ABS,3},{LSR,ABX,3},__illegal__, // 5
    {RTS,IMP,1},{ADC,IZX,2},__illegal__,__illegal__,__illegal__,{ADC,ZPG,2},{ROR,ZPG,2},__illegal__,{PLA,IMP,1},{ADC,IMM,2},{ROR,ACC,1},__illegal__,{JMP,IND,3},{ADC,ABS,3},{ROR,ABS,3},__illegal__, // 6
    {BVS,REL,2},{ADC,IZY,2},__illegal__,__illegal__,__illegal__,{ADC,ZPX,2},{ROR,ZPX,2},__illegal__,{SEI,IMP,1},{ADC,ABY,3},__illegal__,__illegal__,__illegal__,{ADC,ABS,3},{ROR,ABX,3},__illegal__, // 7
    __illegal__,{STA,IZX,2},__illegal__,__illegal__,{STY,ZPG,2},{STA,ZPG,2},{STX,ZPG,2},__illegal__,{DEY,IMP,1},__illegal__,{TXA,IMP,1},__illegal__,{STY,ABS,3},{STA,ABS,3},{STX,ABS,3},__illegal__, // 8
    {BCC,REL,2},{STA,IZY,2},__illegal__,__illegal__,{STY,ZPX,2},{STA,ZPX,2},{STX,ZPX,2},__illegal__,{TYA,IMP,1},{STA,ABY,3},{TXS,IMP,1},__illegal__,__illegal__,{STA,ABS,3},__illegal__,__illegal__, // 9
    {LDY,IMM,2},{LDA,IZX,2},{LDX,IMM,2},__illegal__,{LDY,ZPG,2},{LDA,ZPG,2},{LDX,ZPG,2},__illegal__,{TAY,IMP,1},{LDA,IMM,2},{TAX,IMP,1},__illegal__,{LDY,ABS,3},{LDA,ABS,3},{LDX,ABS,3},__illegal__, // A
    {BCS,REL,2},{LDA,IZY,2},__illegal__,__illegal__,{LDY,ZPX,2},{LDA,ZPX,2},{LDX,ZPX,2},__illegal__,{CLV,IMP,1},{LDA,ABY,3},{TSX,IMP,1},__illegal__,{LDY,ABX,3},{LDA,ABS,3},{LDX,ABX,3},__illegal__, // B
    {CPY,IMM,2},{CMP,IZX,2},__illegal__,__illegal__,{CPY,ZPG,2},{CMP,ZPG,2},{DEC,ZPG,2},__illegal__,{INY,IMP,1},{CMP,IMM,2},{DEX,IMP,1},__illegal__,{CPY,ABS,3},{CMP,ABS,3},{DEC,ABS,3},__illegal__, // C
    {BNE,REL,2},{CMP,IZY,2},__illegal__,__illegal__,__illegal__,{CMP,ZPX,2},{DEC,ZPX,2},__illegal__,{CLD,IMP,1},{CMP,ABY,3},__illegal__,__illegal__,__illegal__,{CMP,ABS,3},{DEC,ABX,3},__illegal__, // D
    {CPX,IMM,2},{SBC,IZX,2},__illegal__,__illegal__,{CPX,ZPG,2},{SBC,ZPG,2},{INC,ZPG,2},__illegal__,{INX,IMP,1},{SBC,IMM,2},{NOP,IMP,1},__illegal__,{CPX,ABS,3},{SBC,ABS,3},{INC,ABS,3},__illegal__, // E
    {BEQ,REL,2},{SBC,IZY,2},__illegal__,__illegal__,__illegal__,{SBC,ZPX,2},{INC,ZPX,2},__illegal__,{SED,IMP,1},{SBC,ABY,3},__illegal__,__illegal__,__illegal__,{SBC,ABS,3},{INC,ABX,3},__illegal__  // F
};

std::string IntToHexString(int number) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << number;
    return ss.str();
}

std::string intToBinary(int num, int bitCount = 8) {
    return std::bitset<32>(num).to_string().substr(32 - bitCount, bitCount);
}

std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string OpcodeToAsm(Opcode opcode, int num) {
    std::string output = "";
    output += InstructionStrings[opcode.name];
    switch(opcode.addressing) {
        case ACC: // opc a
            output += " A";
            break;
        case ABS: // opc $HHLL
            output += " $"+IntToHexString(num);
            break;
        case ABX: // opc $HHLL,x
            output += " $"+IntToHexString(num)+",X";
            break;
        case ABY: // opc $HHLL,y
            output += " $"+IntToHexString(num)+",Y";
            break;
        case IMM: // opc #$BB
            output += " #$"+IntToHexString(num);
            break;
        case IMP: // opc 
            break;
        case IND: // opc ($LLHH)
            output += "($"+IntToHexString(num)+")";
            break;
        case IZX: // opc ($LL,x)
            output += " ($"+IntToHexString(num)+",X)";
            break;
        case IZY: // opc ($LL),y
            output += " ($"+IntToHexString(num)+"),Y";
            break;
        case REL: // opc $BB
            output += " $"+intToBinary(num);
            break;
        case ZPG: // opc $LL
            output += " $"+IntToHexString(num);
            break;
        case ZPX: // opc $LL,x
            output += " $"+IntToHexString(num)+",X";
            break;
        case ZPY: // opc $LL,y
            output += " $"+IntToHexString(num)+",Y";
            break;
    }

    return toLowerCase(output);
}