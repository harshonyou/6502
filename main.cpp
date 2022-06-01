#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize() {
        for(u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    // Read 1 byte
    Byte operator[] (u32 Address) const {
        // Assert here Address is < MAX_MEM
        return Data[Address];
    }

    // Write 1 byte
    Byte& operator[] (u32 Address) {
        // Assert here Address is < MAX_MEM
        return Data[Address];
    }
};

struct CPU {
    Word PC;            // Program Counter
    Byte SP;            // Stack Pointer

    Byte ACC, X, Y;     // Accumulator and Registers

    //TODO Make all the flag a singular struct
    // called status flag with accessible flags
    Byte CF : 1;        // Carry Flag
    Byte ZF : 1;        // Zero Flag
    Byte ID : 1;        // Interrupt Disable
    Byte DM : 1;        // Decimal Mode
    Byte BC : 1;        // Break Command
    Byte VF : 1;        // Overflown Flag
    Byte NF : 1;        // Negative Flag

    void Reset(Mem& mainMemory) {
        PC = 0xFFFC;
        SP = ACC = X = Y = 0;
        CF = ZF = ID = DM = BC = VF = NF = 0;
        mainMemory.Initialize();
    }

    Byte FetchByte(u32& Cycles, Mem& mainMemory) {
        Byte Data = mainMemory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    // Opcodes
    static constexpr Byte INS_LDA_IM = 0xA9;

    void Execute(u32 Cycles, Mem& mainMemory) {
        while (Cycles > 0) {
            Byte Instruction = FetchByte(Cycles, mainMemory);
            switch(Instruction) {
                case INS_LDA_IM: {
                    Byte Value = FetchByte(Cycles, mainMemory);
                    ACC = Value;
                    ZF = (ACC == 0);
                    NF = (ACC & 0b10000000) > 0;
                } break;
                default: {
                    printf("Instruction not handled %d", Instruction);
                } break;
            }
        }
    }
};

int main() {
    printf("\n");
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;
    // end - inline a little program
    cpu.Execute(2, mem);
    return 0;
}