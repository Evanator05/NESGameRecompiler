#include <stdint.h>
namespace CPU {

    struct CPUState {
        uint16_t PC; // Program Counter
        uint8_t A; // Accumulator
        uint8_t X; // X Register
        uint8_t Y; // Y Register
        uint8_t SR; // Status Register
        uint8_t SP; // Stack Pointer
        uint8_t memory[4096]; // 4k of ram
    } CPUState;
    // Status Register Flags
    enum StatusRegisterFlags {
        N = 1<<7, // Negative
        V = 1<<6, // Overflow
        _ = 1<<5, // Ignored
        B = 1<<4, // Break
        D = 1<<3, // Decimal
        I = 1<<2, // Interrupt
        Z = 1<<1, // Zero
        C = 1     // Carry
    };

    void SetFlag(uint8_t flag, bool state) {
        if (state) {
            CPUState.SR |= flag;
        } else {
            CPUState.SR &= ~flag;
        }
    }

    // Addressing Modes    
    void ACC() {

    }
    void ABS() {

    }
    void ABX() {

    }
    void ABY() {

    }
    void IMM() {

    }
    void IMP() {

    }
    void IND() {

    }
    void INX() {

    }
    void INY() {

    }
    void REL() {

    }
    void ZPG() {

    }
    void ZPX() {

    }
    void ZPY() {

    }

    // Instructions

    //Transfer Instructions
    void LDA(uint8_t& M) {
        CPUState.A = M;
    }
    void LDX(uint8_t& M) {
        CPUState.X = M;
    }
    void LDY(uint8_t& M) {
        CPUState.Y = M;
    }
    void STA(uint8_t& M) {
        M = CPUState.A;
    }
    void STX(uint8_t& M) {
        M = CPUState.X;
    }
    void STY(uint8_t& M) {
        M = CPUState.Y;
    }
    void TAX() {
        CPUState.X = CPUState.A;
    }
    void TAY() {
        CPUState.Y = CPUState.A;
    }
    void TSX() {
        CPUState.X = CPUState.SP;
    }
    void TXA() {
        CPUState.A = CPUState.X;
    }
    void TXS() {
        CPUState.X = CPUState.SP;
    }
    void TYA() {
        CPUState.A = CPUState.Y;
    }
    // Stack Instructions
    void PHA() {

    }
    void PHP() {

    }
    void PLA() {

    }
    void PLP() {

    }
    // Decrements & Increments
    void DEC(uint8_t& M) {
        M -= 1;
    }
    void DEX() {
        CPUState.X -= 1;
    }
    void DEY() {
        CPUState.Y -= 1;
    }
    void INC(uint8_t& M) {
        M += 1;
    }
    void INX() {
        CPUState.X += 1;
    }
    void INY() {
        CPUState.Y += 1;
    }
    // Arithmetic Operations
    void ADC() {

    }
    void SBC() {

    }
    // Logical Operations
    void AND(uint8_t& M) {
        CPUState.A &= M;
    }
    void EOR(uint8_t& M) {
        CPUState.A ^= M;
        SetFlag(N, CPUState.A & N);
    }
    void ORA(uint8_t& M) {
        CPUState.A |= M;
    }
    // Shift & Rotate Instructions
    void ASL() {

    }
    void LSR() {

    }
    void ROL() {

    }
    void ROR() {

    }
    // Flag Instructions
    void CLC() {
        SetFlag(C, false);
    }
    void CLD() {
        SetFlag(D, false);
    }
    void CLI() {
        SetFlag(I, false);
    }
    void CLV() {
        SetFlag(V, false);
    }
    void SEC() {
        SetFlag(C, true);
    }
    void SED() {
        SetFlag(D, true);
    }
    void SEI() {
        SetFlag(I, true);
    }
    // Comparisons
    void CMP() {

    }
    void CPX() {

    }
    void CPY() {

    }
    // Conditional Branch Instructions
    void BCC() {

    }
    void BCS() {

    }
    void BEQ() {

    }
    void BMI() {

    }
    void BNE() {

    }
    void BPL() {

    }
    void BVC() {

    }
    void BVS() {

    }
    // Jumps & Subroutines
    void JMP() {

    }
    void JSR() {

    }
    void RTS() {

    }
    // Interrupts
    void BRK() {

    }
    void RTI() {

    }
    void BIT() {

    }
    void NOP() {

    }
}