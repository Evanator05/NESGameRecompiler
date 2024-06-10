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

uint8_t AddressingModeBytes[] = {
    1, // ACC
    3, // ABS
    3, // ABX
    3, // ABY
    2, // IMM
    1, // IMP
    3, // IND
    2, // IZX
    2, // IZY
    2, // REL
    2, // ZPG
    2, // ZPX
    2 // ZPY
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
};
Opcode _illegal_ = {XXX,IMP};

Opcode opcodeTable[] =
{// 0         1         2         3         4         5         6         7         8         9         A         B         C         D         E         F
    {BRK,IMP},{ORA,IZX},_illegal_,_illegal_,_illegal_,{ORA,ZPG},{ASL,ZPG},_illegal_,{PHP,IMP},{ORA,IMM},{ASL,ACC},_illegal_,_illegal_,{ORA,ABS},{ASL,ABS},_illegal_, // 0
    {BPL,REL},{ORA,IZY},_illegal_,_illegal_,_illegal_,{ORA,ZPX},{ASL,ZPX},_illegal_,{CLC,IMP},{ORA,ABY},_illegal_,_illegal_,_illegal_,{ORA,ABS},{ASL,ABX},_illegal_, // 1
    {JSR,ABS},{AND,IZX},_illegal_,_illegal_,{BIT,ZPG},{AND,ZPG},{ROL,ZPG},_illegal_,{PLP,IMP},{AND,IMM},{ROL,ACC},_illegal_,{BIT,ABS},{AND,ABS},{ROL,ABS},_illegal_, // 2
    {BMI,REL},{AND,IZY},_illegal_,_illegal_,_illegal_,{AND,ZPX},{ROL,ZPX},_illegal_,{SEC,IMP},{AND,ABY},_illegal_,_illegal_,_illegal_,{AND,ABS},{ROL,ABX},_illegal_, // 3
    {RTI,IMP},{EOR,IZX},_illegal_,_illegal_,_illegal_,{EOR,ZPG},{LSR,ZPG},_illegal_,{PHA,IMP},{EOR,IMM},{LSR,ACC},_illegal_,{JMP,ABS},{EOR,ABS},{LSR,ABS},_illegal_, // 4
    {BVC,REL},{EOR,IZY},_illegal_,_illegal_,_illegal_,{EOR,ZPX},{LSR,ZPX},_illegal_,{CLI,IMP},{EOR,ABY},_illegal_,_illegal_,_illegal_,{EOR,ABS},{LSR,ABX},_illegal_, // 5
    {RTS,IMP},{ADC,IZX},_illegal_,_illegal_,_illegal_,{ADC,ZPG},{ROR,ZPG},_illegal_,{PLA,IMP},{ADC,IMM},{ROR,ACC},_illegal_,{JMP,IND},{ADC,ABS},{ROR,ABS},_illegal_, // 6
    {BVS,REL},{ADC,IZY},_illegal_,_illegal_,_illegal_,{ADC,ZPX},{ROR,ZPX},_illegal_,{SEI,IMP},{ADC,ABY},_illegal_,_illegal_,_illegal_,{ADC,ABS},{ROR,ABX},_illegal_, // 7
    _illegal_,{STA,IZX},_illegal_,_illegal_,{STY,ZPG},{STA,ZPG},{STX,ZPG},_illegal_,{DEY,IMP},_illegal_,{TXA,IMP},_illegal_,{STY,ABS},{STA,ABS},{STX,ABS},_illegal_, // 8
    {BCC,REL},{STA,IZY},_illegal_,_illegal_,{STY,ZPX},{STA,ZPX},{STX,ZPX},_illegal_,{TYA,IMP},{STA,ABY},{TXS,IMP},_illegal_,_illegal_,{STA,ABS},_illegal_,_illegal_, // 9
    {LDY,IMM},{LDA,IZX},{LDX,IMM},_illegal_,{LDY,ZPG},{LDA,ZPG},{LDX,ZPG},_illegal_,{TAY,IMP},{LDA,IMM},{TAX,IMP},_illegal_,{LDY,ABS},{LDA,ABS},{LDX,ABS},_illegal_, // A
    {BCS,REL},{LDA,IZY},_illegal_,_illegal_,{LDY,ZPX},{LDA,ZPX},{LDX,ZPX},_illegal_,{CLV,IMP},{LDA,ABY},{TSX,IMP},_illegal_,{LDY,ABX},{LDA,ABS},{LDX,ABX},_illegal_, // B
    {CPY,IMM},{CMP,IZX},_illegal_,_illegal_,{CPY,ZPG},{CMP,ZPG},{DEC,ZPG},_illegal_,{INY,IMP},{CMP,IMM},{DEX,IMP},_illegal_,{CPY,ABS},{CMP,ABS},{DEC,ABS},_illegal_, // C
    {BNE,REL},{CMP,IZY},_illegal_,_illegal_,_illegal_,{CMP,ZPX},{DEC,ZPX},_illegal_,{CLD,IMP},{CMP,ABY},_illegal_,_illegal_,_illegal_,{CMP,ABS},{DEC,ABX},_illegal_, // D
    {CPX,IMM},{SBC,IZX},_illegal_,_illegal_,{CPX,ZPG},{SBC,ZPG},{INC,ZPG},_illegal_,{INX,IMP},{SBC,IMM},{NOP,IMP},_illegal_,{CPX,ABS},{SBC,ABS},{INC,ABS},_illegal_, // E
    {BEQ,REL},{SBC,IZY},_illegal_,_illegal_,_illegal_,{SBC,ZPX},{INC,ZPX},_illegal_,{SED,IMP},{SBC,ABY},_illegal_,_illegal_,_illegal_,{SBC,ABS},{INC,ABX},_illegal_  // F
};

std::string IntToHexString(int number) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << number;
    return ss.str();
}

std::string intToBinary(int num, int bitCount = 8) {
    return std::bitset<32>(num).to_string().substr(32 - bitCount, bitCount);
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),[](unsigned char c) { return std::tolower(c); });
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
    return toLower(output);
}

