#include "LC4Machine.h"

unsigned short
LC4Machine::barrel_roll(unsigned short op)
{
    unsigned short oper_type = extract(op, 4, 5);
    unsigned short d = extract(op, 9, 11);
    unsigned short s = extract(op, 6, 8);
    unsigned short t = extract(op, 0, 2);
    unsigned short U = extract(op, 0, 3);
    switch (oper_type)
    {
    case 0:
        // shift left logical
        this->regs.r[d] = this->regs.r[s] << U;
        break;
    case 1:
        // shift right arithmetic
        {
        short x = (short) this->regs.r[s];
        x >>= U;
        this->regs.r[d] = (unsigned short) x;
        }
        break;
    case 2:
        // shift right logical
        this->regs.r[d] = this->regs.r[s] >> U;
        break;
    case 3:
        // mod
        this->regs.r[d] = this->regs.r[s] % this->regs.r[t];
        break;
    }

    // update NZP flags
    this->update_NZP(this->regs.r[d]);

    return this->regs.r[d];
}
