#include "LC4Machine.h"
#include "ali.h"
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

ALI::ALI(std::string src_filename)
{
    if (src_filename.empty()) {
        this->NO_SOURCE_FILE = true;
    }
    else {
        this->NO_SOURCE_FILE = false;
        std::ifstream in(src_filename.c_str(), std::ios::in);
        if (in.fail()) {
            std::cout << "BIN file does not exist!";
            in.close();
            goto ali_ctor_continue;
        }

        while (in.good()) {
            std::string s;
            std::getline(in, s);
            if (!s.empty()) {
                this->code.push_back(s);
            }
        }

        in.close();

    }

ali_ctor_continue:

    // store LC4Machine reference
    this->memory_location = 0;

    // initialized ncurses
    initscr();
    // turn off character echo
    //noecho();
    // Line buffering disabled
    //cbreak();
    // enable keys like F1
    //keypad(stdscr, TRUE);

    // initialize windows
    this->wRegisters = newwin(24, 11, 0, 0);
    this->wMemory = newwin(7, 69, 0, 11);
    this->wDisassembly = newwin(13, 69, 7, 11);
    this->wCommandLine = newwin(4, 69, 20, 11);
    this->commandLineMsg = "";

}

ALI::~ALI()
{
    // cleanup ncurses
    endwin();

}

void
ALI::loop()
{
    while (1) {
        char str[1000];
        wgetnstr(this->wCommandLine, str, 1000);
        if (!this->parseCommand(str)) {
            break;
        }
        this->redraw();
    }
}

bool
ALI::parseCommand(char* str)
{
    static std::string lastCommand = "";

    if (strlen(str) == 0) {
        strcpy(str, lastCommand.c_str());
    }

    if (strcmp(str, "quit") == 0 || strcmp(str, "q") == 0 || strcmp(str, "exit") == 0) {
        return false;
    }
    else if (strstr(str, "mem") == str) {
        // Make sure there is at least a number after the mem command
        if (strlen(str) < 5) {
            return true;
        }
        unsigned short new_mem_loc = strtol(&str[4], NULL, 16);
        this->memory_location = new_mem_loc;
        char buf[1000];
        sprintf(buf, "Reading VM memory address 0x%04x from address %p", new_mem_loc, &this->lc4->memory[new_mem_loc]);
        this->commandLineMsg = std::string("") + buf;
    }
    else if (strcmp(str, "step") == 0 || strcmp(str, "s") == 0) {
        this->commandLineMsg = "";
        bool b = this->lc4->step();
        if (!b) {
            this->commandLineMsg = "End of code reached. Please restart or exit.";
        }
    }
    else if (strstr(str, "step ") == str)  {
        if (strlen(str) < 6) {
            return true;
        }
        this->commandLineMsg = "";
        unsigned int steps = strtol(&str[5], NULL, 16);
        for (unsigned int ii = 0; ii < steps; ii++) {
            bool b = this->lc4->step();
            if (!b) {
                this->commandLineMsg = "End of code reached. Please restart or exit.";
                break;
            }
        }
    }
    else if (strstr(str, "s ") == str) {
        if (strlen(str) < 3) {
            return true;
        }
        this->commandLineMsg = "";
        unsigned int steps = strtol(&str[2], NULL, 16);
        for (unsigned int ii = 0; ii < steps; ii++) {
            bool b = this->lc4->step();
            if (!b) {
                this->commandLineMsg = "End of code reached. Please restart or exit.";
                break;
            }
        }
    }
    else if (strcmp(str, "r") == 0 || strcmp(str, "restart") == 0) {
        this->lc4->init();
        this->commandLineMsg = "LC4 Virtual Machine reinitialized.";
    }
    else {
        this->commandLineMsg = "Invalid command entered";
    }

    lastCommand = str;

    return true;
}

void
ALI::redraw()
{
    LC4::registers regs(this->lc4->get_registers());

    // output registers
    box(this->wRegisters, 0, 0);
    mvwprintw(this->wRegisters, 1, 1, " R0=%04x", regs.r[0]);
    mvwprintw(this->wRegisters, 2, 1, " R1=%04x", regs.r[1]);
    mvwprintw(this->wRegisters, 3, 1, " R2=%04x", regs.r[2]);
    mvwprintw(this->wRegisters, 4, 1, " R3=%04x", regs.r[3]);
    mvwprintw(this->wRegisters, 5, 1, " R4=%04x", regs.r[4]);
    mvwprintw(this->wRegisters, 6, 1, " R5=%04x", regs.r[5]);
    mvwprintw(this->wRegisters, 7, 1, " R6=%04x", regs.r[6]);
    mvwprintw(this->wRegisters, 8, 1, " R7=%04x", regs.r[7]);

    mvwprintw(this->wRegisters, 10, 1, " PC=%04x", regs.pc);

    mvwprintw(this->wRegisters, 12, 1, "MPR=%04x", regs.mpr);
    mvwprintw(this->wRegisters, 13, 1, "PSR=%04x", regs.psr);

    int zero = 0;
    int one = 1;
    mvwprintw(this->wRegisters, 15, 1, "  N=%d", regs.n ? one : zero);
    mvwprintw(this->wRegisters, 16, 1, "  Z=%d", regs.z ? one : zero);
    mvwprintw(this->wRegisters, 17, 1, "  P=%d", regs.p ? one : zero);

    // print memory region
    // we have 5 lines to play with
    box(this->wMemory, 0, 0);

    unsigned short loc = this->memory_location;
    for (int row = 1; row <= 5; row++) {
        mvwprintw(this->wMemory, row, 1, "%04x: %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x", loc,
                  this->lc4->get_mem(loc),
                  this->lc4->get_mem(loc+1),
                  this->lc4->get_mem(loc+2),
                  this->lc4->get_mem(loc+3),
                  this->lc4->get_mem(loc+4),
                  this->lc4->get_mem(loc+5),
                  this->lc4->get_mem(loc+6),
                  this->lc4->get_mem(loc+7),
                  this->lc4->get_mem(loc+8),
                  this->lc4->get_mem(loc+9),
                  this->lc4->get_mem(loc+10),
                  this->lc4->get_mem(loc+11)
                  );
        loc += 12;
    }


    // print disassembly
    wclear(this->wDisassembly);
    box(this->wDisassembly, 0, 0);
    for (int row = 1; row <= 11; row++) {
        unsigned short insn_ptr = regs.pc + row - 1;
        std::string insn_str;
        try {
            insn_str = this->code.at(insn_ptr);
        }
        catch(...) {
            insn_str = "---";
        }
        unsigned short insn = this->lc4->get_mem(insn_ptr);
        std::string binary = "";
        std::string empty = "";
        unsigned short mask = 1;
        for (int ii = 0; ii < 16; ii++) {
            if (mask & insn) {
                binary = empty + "1" + binary;
            }
            else {
                binary = empty + "0" + binary;
            }
            mask <<= 1;
        }
        mvwprintw(this->wDisassembly, row, 1, "%04x:\t%s    %s", insn_ptr, binary.c_str(), insn_str.c_str());
    }

    // print command line region
    wclear(this->wCommandLine);
    box(this->wCommandLine, 0, 0);
    mvwprintw(this->wCommandLine, 1, 1, this->commandLineMsg.c_str());
    mvwprintw(this->wCommandLine, 2, 1, "> ");

    // refresh windows
    wrefresh(wRegisters);
    wrefresh(wMemory);
    wrefresh(wCommandLine);
    wrefresh(wDisassembly);

    // position cursor to correct location
    wmove(this->wCommandLine, 2, 3);

    refresh();
}

int
ALI::begin(int argc, char* argv[])
{
    if (argc <= 1 || argc > 3) {
        printf("%s [bin file] (optional source file)\n", argv[0]);
        return 1;
    }

    std::string bin_file = argv[1];
    std::string src_file = "";
    if (argc == 3) {
        src_file = argv[2];
    }

    try {
        LC4Machine lc4(bin_file);
        ALI ali(src_file);
        lc4.setManagedBy(&ali);
        ali.setMachine(&lc4);
        ali.loop();
    }
    catch(const char* str) {
        std::cout << str << std::endl;
        return 1;
    }
    catch(...) {
        std::cout << "Uncaught exception" << std::endl;
        return 255;
    }

    return 0;
}
