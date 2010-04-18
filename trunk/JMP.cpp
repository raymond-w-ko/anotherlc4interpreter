#include "LC4Machine.h"

unsigned short
LC4Machine::perform_jmp(unsigned short op)
{
    unsigned int bit = extract(op, 11, 11);
    if (bit) {      // JMPR
        this->regs.pc = this->regs.r[extract(op, 6, 8)];

        // to offset the global increment at the end of step();
        this->regs.pc--;
    }
    else {          // JMP
        unsigned short I = SEXT(extract(op, 0, 10), 11);
        this->regs.pc += I;
    }

    // What the value should be, but pc++ hasn't occurred globally yet
    return this->regs.pc + 1;
}
