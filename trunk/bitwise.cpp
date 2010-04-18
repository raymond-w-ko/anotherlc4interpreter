#include "LC4Machine.h"

unsigned short
LC4Machine::perform_bitwise(unsigned short op)
{
    unsigned short oper_type = extract(op, 3, 5);
    unsigned short d = extract(op, 9, 11);
    unsigned short s = extract(op, 6, 8);
    unsigned short t = extract(op, 0, 2);
    switch (oper_type)
    {
    case 0:
        // and
        this->regs.r[d] = this->regs.r[s] & this->regs.r[t];
        break;
    case 1:
        // not
        this->regs.r[d] = ~this->regs.r[s];
        break;
    case 2:
        // or
        this->regs.r[d] = this->regs.r[s] | this->regs.r[t];
        break;
    case 3:
        // xor
        this->regs.r[d] = this->regs.r[s] ^ this->regs.r[t];
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        // and const
        this->regs.r[d] = this->regs.r[s] & SEXT(extract(op, 0, 4), 5);
        break;
    }

    // update NZP flags
    this->update_NZP(this->regs.r[d]);

    return this->regs.r[d];
}
