#pragma once
#include <ncurses.h>

class ALI
{
public:
    ALI(LC4Machine* lc4);
    ~ALI();

    void loop();
private:
    LC4Machine* lc4;

    WINDOW* wRegisters;
    WINDOW* wMemory;
    WINDOW* wCommandLine;

    void redraw();

    unsigned short memory_location;
};
