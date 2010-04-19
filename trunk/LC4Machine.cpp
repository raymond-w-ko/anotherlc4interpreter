#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include "LC4Machine.h"

LC4Machine::LC4Machine(std::string filename)
{
    // Initialize all registers to 0x0
    memset(this->regs.r, 0, sizeof(this->regs.r));
    this->regs.r[0] = 1;
    this->regs.r[1] = 2;
    this->regs.r[2] = 3;
    this->regs.r[3] = 4;
    this->regs.r[4] = 5;
    this->regs.r[5] = 6;
    this->regs.r[6] = 7;
    this->regs.r[7] = 8;

    this->regs.pc = 0x0000;

    this->regs.mpr = 0x0000;
    this->regs.psr = 0x0000;

    this->regs.n = false;
    this->regs.z = false;
    this->regs.p = false;

    // Initialize memory to 0
    memset(this->memory, 0, sizeof(this->memory));

    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if (in.fail()) {
        throw "BIN file does not exist!";
    }

    // Load instructions into memory space starting at 0x0;
    if (filename.empty()) {
        throw "Empty string filename supplied!";
    }

    unsigned int counter = 0;
    while (in.good()) {
        unsigned short insn = 0x0;
        in.read(reinterpret_cast<char*>(&insn), 2);
        if (!in.eof()) {
            insn = ((insn & 0xFF00) >> 8) | ((insn & 0x00FF) << 8);
            this->memory[counter++] = insn;
            if (counter > 0xFFFF) {
                throw "LC4 Machine Out of Memory Error!";
            }
        }
    }
    this->end_of_instructions = counter;

    in.close();
}

bool
LC4Machine::step()
{
    // Check to see if we have reached end of instruction byte
    if (this->regs.pc == this->end_of_instructions) {
        return false;
    }

    // Fetch instruction
    short insn = this->memory[this->regs.pc];

    // Decode type of instruction
    unsigned short f4b = (insn & 0xF000) >> 12;     // first 4 bytes

    switch(f4b) {
    case 0:
        // branch
        this->perform_branch(insn);
        break;
    case 1:
        // ALU
        this->perform_ALU(insn);
        break;
    case 2:
        // compare
        this->perform_compare(insn);
        break;
    case 4:
        // absolute jump
        this->perform_jsr(insn);
        break;
    case 5:
        // bitwise operations
        this->perform_bitwise(insn);
        break;
    case 6:
        // load
        this->perform_load(insn);
        break;
    case 7:
        // store
        this->perform_store(insn);
        break;
    case 8:
        // RTI
        this->regs.pc = this->regs.r[7];
        // offset the global increment at the end
        this->regs.pc--;

        // Turn off MSB of psr
        this->regs.psr = ~this->regs.psr;
        this->regs.psr |= 0x8000;
        this->regs.psr = ~this->regs.psr;
        break;
    case 9:
        // load a constant into register
        {
        unsigned short d = extract(insn, 9, 11);
        unsigned short I = SEXT(extract(insn, 0, 8), 9);
        this->regs.r[d] = I;
        this->update_NZP(this->regs.r[d]);
        }
        break;
    case 10:
        // shift + modulus
        this->barrel_roll(insn);
        break;
    case 12:
        // absolute jump without saving to register 7
        // relative jump
        this->perform_jmp(insn);
        break;
    case 13:
        // load a byte constant into the higher bits of a register
        {
        unsigned short d = extract(insn, 9, 11);
        unsigned short U = extract(insn, 0, 7);
        this->regs.r[d] = (this->regs.r[d] & 0x00FF) | (U << 8);
        this->update_NZP(this->regs.r[d]);
        }
        break;
    case 15:
        // trap instruction
        // mostly unsupported
        this->regs.r[7] = this->regs.pc + 1;
        this->update_NZP(this->regs.r[7]);
        this->regs.pc = 0x8000 | extract(insn, 0, 7);

        // Turn on MSB of psr
        this->regs.psr |= 0x8000;
        break;
    }

    // Increment instruction pointer
    this->regs.pc++;

    return true;
}

unsigned short
LC4Machine::extract(unsigned short insn, unsigned int lsb, unsigned msb)
{
    unsigned short mask = 0;
    for (unsigned int ii = lsb; ii <= msb; ii++) {
        unsigned int bit = 1 << ii;
        mask |= bit;
    }

    return (insn & mask) >> lsb;
}

unsigned short
LC4Machine::SEXT(unsigned short n, unsigned int size)
{
    unsigned int bit = (n & (1 << (size - 1))) >> (size - 1);
    if (bit) {
        for (unsigned int ii = size; ii <= 15; ii++) {
            unsigned int bit2 = 1 << ii;
            n |= bit2;
        }
    }
    else {
        n = ~n;
        for (unsigned int ii = size; ii <= 15; ii++) {
            unsigned int bit2 = 1 << ii;
            n |= bit2;
        }
        n = ~n;
    }

    return n;
}

void
LC4Machine::update_NZP(unsigned short n)
{
    short twos_complement = (short) n;
    (twos_complement == 0) ? this->regs.z = true : this->regs.z = false;
    (twos_complement > 0) ? this->regs.p = true : this->regs.p = false;
    (twos_complement < 0) ? this->regs.n = true : this->regs.n = false;
}
