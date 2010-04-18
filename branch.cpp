#include "LC4Machine.h"

unsigned short
LC4Machine::perform_branch(unsigned short op)
{
    short I = (short) SEXT(extract(op, 0, 8), 9);
    unsigned short oper_type = extract(op, 10, 12);
    switch(oper_type) {
    case 0:     // NOP
        break;
    case 4:     // BRn
        this->regs.n ? this->regs.pc += I : 1;
        break;
    case 6:     // BRnz
        this->regs.n || this->regs.z ? this->regs.pc += I : 1;
        break;
    case 5:     // BRnp
        this->regs.n || this->regs.p ? this->regs.pc += I : 1;
        break;
    case 2:     // BRz
        this->regs.z ? this->regs.pc += I : 1;
        break;
    case 3:     // BRzp
        this->regs.z || this->regs.p ? this->regs.pc += I : 1;
        break;
    case 1:     // BRp
        this->regs.p ? this->regs.pc += I : 1;
        break;
    case 7:     // BRnzp - always jump
        this->regs.pc += I;
        break;
    }

    // What the value should be, but pc++ hasn't occurred globally yet
    return this->regs.pc + 1;
}
