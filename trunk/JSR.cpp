#include "LC4Machine.h"

unsigned short
LC4Machine::perform_jsr(unsigned short op)
{
    unsigned short oper_type = extract(op, 11, 11);
    if (oper_type) {
        // JSR
        this->regs.r[7] = this->regs.pc + 1;
        this->regs.pc = (this->regs.pc & 0x8000) | (extract(op, 0, 10) << 4);
    }
    else {
        // JSRR
        this->regs.r[7] = this->regs.pc + 1;
        this->regs.pc = this->regs.r[extract(op, 6, 8)];
    }

    this->update_NZP(this->regs.r[7]);

    // this is to offset the global increment at the end of step();
    this->regs.pc--;

    // what the value should be but pc++ hasn't occurred globally yet;
    return this->regs.pc++;
}
