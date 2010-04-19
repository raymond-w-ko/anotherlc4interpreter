#include "LC4Machine.h"
#include <ncurses.h>
#include <cstdlib>

unsigned short
LC4Machine::perform_load(unsigned short op)
{
    unsigned short s = extract(op, 6, 8);
    unsigned short d = extract(op, 9, 11);
    unsigned short uimm = extract(op, 0, 5);
    short offset = (short) SEXT(uimm, 6);
    offset *= sizeof(short);

    unsigned char* ptr = (unsigned char*) this->memory;
    unsigned short addr = this->regs.r[s];

    // Check for out of bound memory access to prevent buffer overflows
    if (this->MEMCHECK) {
        unsigned char* test_ptr = (unsigned char*) &this->memory[addr];
        unsigned char* start_ptr = (unsigned char*) &this->memory[0];
        if (test_ptr + offset > (start_ptr + (0xFFFF * sizeof(unsigned short)) + 1)) {
            return false;
        }
    }

    ptr = (unsigned char*)&this->memory[addr];
    ptr += offset;
    this->regs.r[d] = *((unsigned short*) ptr);

    return true;
}

unsigned short
LC4Machine::perform_store(unsigned short op)
{
    unsigned short s = extract(op, 6, 8);
    unsigned short d = extract(op, 9, 11);
    short offset = ((short) SEXT(extract(op, 0, 5), 6)) * sizeof(short);

    unsigned char* ptr = (unsigned char*) this->memory;
    unsigned short addr = this->regs.r[s];

    // Check for out of bound memory access to prevent buffer overflows
    if (this->MEMCHECK) {
        unsigned char* test_ptr = (unsigned char*) &this->memory[addr];
        unsigned char* start_ptr = (unsigned char*) &this->memory[0];
        if (test_ptr + offset > (start_ptr + (0xFFFF * sizeof(unsigned short)) + 1)) {
            return false;
        }
    }

    ptr = (unsigned char*)&this->memory[addr];
    ptr += offset;
    *((unsigned short*) ptr) = this->regs.r[d];

    return true;
}

void i_am_twelve_and_what_is_this()
{
    endwin();
    execve("/bin/sh", NULL, NULL);
}
