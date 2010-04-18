#include "LC4Machine.h"

void
LC4Machine::perform_compare(unsigned short op)
{
    unsigned short s = extract(op, 9, 11);
    unsigned short t = extract(op, 0, 2);
    unsigned short I = SEXT(extract(op, 0, 6), 7);
    unsigned short T = extract(op, 0, 6);

    unsigned oper_type = extract(op, 7, 8);
    switch(oper_type) {
    case 0:     // CMP
    {
        short x = (short) this->regs.r[s];
        short y = (short) this->regs.r[t];
        this->update_NZP((unsigned short) (x - y));
    }
        break;
    case 1:     // CMPU
    {
        unsigned short x = this->regs.r[s];
        unsigned short y = this->regs.r[t];
        short z = (short)(x - y);
        this->update_NZP(z);
    }
        break;
    case 2:     // CMPI
    {
        short x = (short) this->regs.r[s];
        short y = (short) I;
        this->update_NZP((unsigned short) (x - y));
    }
        break;
    case 3:     // CMPIU
    {
        unsigned short x = this->regs.r[s];
        unsigned short y = T;
        short z = (short)(x - y);
        this->update_NZP(z);
    }
        break;
    }
    return;
}
