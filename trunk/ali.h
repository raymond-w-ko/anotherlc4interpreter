#pragma once
#include "LC4Machine.h"
#include <vector>
#include <string>
#include <ncurses.h>

class LC4Machine;
class ALI
{
friend class LC4Machine;
public:
    ALI(std::string src_filename);
    ~ALI();

    void setMachine(LC4Machine* lc4) { 
        this->lc4 = lc4; 
        refresh();
        this->redraw();
        refresh();
        this->redraw();
        refresh();
    }

    static int begin(int argc, char* argv[]);
    void loop();
private:
    LC4Machine* lc4;

    bool NO_SOURCE_FILE;
    std::vector<std::string> code;

    WINDOW* wRegisters;
    WINDOW* wMemory;
    WINDOW* wDisassembly;
    WINDOW* wCommandLine;
    std::string commandLineMsg;

    void redraw();
    bool parseCommand(char* str);

    unsigned short memory_location;
};
