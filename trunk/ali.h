#pragma once
#include <string>
#include <ncurses.h>

class ALI
{
public:
    ALI(LC4Machine* lc4, std::string src_filename);
    ~ALI();

    void loop();
private:
    LC4Machine* lc4;
    bool NO_SOURCE_FILE;
    std::vector<std::string> code;

    WINDOW* wRegisters;
    WINDOW* wMemory;
    WINDOW* wDisassembly;
    WINDOW* wCommandLine;

    void redraw();
    bool parseCommand(char* str);

    unsigned short memory_location;
};
