#pragma once
#include "ali.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

namespace LC4
{
    class registers
    {
    public:
        unsigned int short r[8];

        unsigned int short pc;

        unsigned int short mpr;
        unsigned int short psr;

        bool n;
        bool z;
        bool p;
    };
}

class ALI;
class LC4Machine
{
public:
    /**
     * Constructor for an LC4 virtual machine that takes
     * a name of a file that contains instructions to execute
     */
    LC4Machine(std::string filename);

    /**
     * Used to initialize or reinitialize the machine
     */
    void init();

    /**
     * Returns a copy of register values
     */
    LC4::registers get_registers() { return LC4::registers(regs); }

    /**
     * Returns the value at a specified memory address
     */
    unsigned short get_mem(unsigned short address) { return this->memory[address]; }

    /**
     * Steps the LC4 Machine through one instruction
     * @return true if no errors have occurred, false 
     * if an error has occurred
     */
    bool step(); 

    /**
     * Extracts a sequence of bits and converts it into an unsigned short
     */
    unsigned short extract(unsigned short insn, unsigned int lsb, unsigned msb);

    /**
     * Sign extension
     */
    unsigned short SEXT(unsigned short n, unsigned int size);
        
    void setManagedBy(ALI* ali) { this->ali = ali; }

private:
    ALI* ali;

    /** Filename that contains binary code */
    std::string filename;

    /** The state of the machine */
    LC4::registers regs;

    /**
     * This variables controls checks for OoB memory reads and writes
     * There is no reason to turn this off
     * maybe for performance reasons later?
     */
    unsigned int MEMCHECK;
    /** Memory of the machine */
    // + 1 is necessary because 0xFFFF is also addressable
    // 0xFFFF means only up to 0xFFFE is addressable
    unsigned short memory[0xFFFF + 1];

    /** location of end of instructions short */
    unsigned short end_of_instructions;

    /** given a value, updates the NZP appropriately */
    void update_NZP(unsigned short n);

    /**
     * Performs a branch instruction
     *
     * @return the new or old PC
     */
    unsigned short perform_branch(unsigned short op);
        
    /**
     * Performs an ALU instruction
     *
     * @return the result that was stored to dst (should be used for debug purposes only)
     */
    unsigned short perform_ALU(unsigned short op);

    /**
     * performs a compare instruction
     */
    void perform_compare(unsigned short op);

    /**
     * performs an absolute jump instruction
     *
     * @returns the place to jump to
     */
    unsigned short perform_jsr(unsigned short op);

    /**
     * performs a bitwise computation
     *
     * @return the result that was stored to dst (should be used for debug purposes only)
     */
    unsigned short perform_bitwise(unsigned short op);

    /**
     * performs memory access operations
     *
     * @returns value stored or retrieved (should be used for debug purposes only)
     */
    unsigned short perform_load(unsigned short op);
    unsigned short perform_store(unsigned short op);

    /**
     * does a barrel roll
     *
     * @return what happens when you do a barrel roll
     */
    unsigned short barrel_roll(unsigned short op);

    /**
     * performs the JMP* instructions
     *
     * @returns the new value of PC
     */
    unsigned short perform_jmp(unsigned short op);

        
    /**
     * Performs a Boolean instruction
     * @param operation the opcode parsed from the instruction
     * @param dst the destination register for storing the result of the op
     * @param src the source register we will perform the operation on
     * @param t the register we will manipulate in conjunction with src OR
     *          the immediate value (as in and ADDI instruction)
     * @return the result that was stored to dst (should be used for debug
     *         purposes only)
     */
    unsigned int perform_BOOL(unsigned short operations, 
                              short dst, short src, short t);
        
};
