#include "LC4Machine.h"

unsigned short
LC4Machine::perform_load(unsigned short op)
{
    unsigned short s = extract(op, 6, 8);
    unsigned short d = extract(op, 9, 11);
    short offset = ((short) SEXT(extract(op, 0, 5), 6)) * sizeof(short);

    void* ptr = this->memory;
    unsigned short addr = this->regs.r[s];

    // Check for out of bound memory access to prevent buffer overflows
    if (this->MEMCHECK) {
        void* test_ptr = (void*) &this->memory[addr];
        if (test_ptr + offset > (test_ptr + 0xFFFF + 1)) {
            throw "Out of bound read error!";
            return 0xDEAD;
        }
    }

    ptr = &this->memory[addr];
    ptr += offset;
    this->regs.r[d] = *((unsigned short*) ptr);

    return this->regs.r[d];
}

unsigned short
LC4Machine::perform_store(unsigned short op)
{
    unsigned short s = extract(op, 6, 8);
    unsigned short d = extract(op, 9, 11);
    short offset = ((short) SEXT(extract(op, 0, 5), 6)) * sizeof(short);

    void* ptr = this->memory;
    unsigned short addr = this->regs.r[s];

    // Check for out of bound memory access to prevent buffer overflows
    if (this->MEMCHECK) {
        void* test_ptr = (void*) &this->memory[addr];
        if (test_ptr + offset > (test_ptr + 0xFFFF + 1)) {
            throw "Out of bound read error!";
            return 0xDEAD;
        }
    }

    ptr = &this->memory[addr];
    ptr += offset;
    *((unsigned short*) ptr) = this->regs.r[d];

    return this->regs.r[d];
}
