#include "LC4Machine.h"
#include "ali.h"
#include <cstring>
#include <ncurses.h>

ALI::ALI(LC4Machine* lc4)
{
    // store LC4Machine reference
    this->lc4 = lc4;
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
    this->wRegisters = newwin(25, 11, 0, 0);
    this->wMemory = newwin(7, 69, 0, 11);
    this->wCommandLine = newwin(18, 69, 7, 11);

    refresh();
    this->redraw();
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
        wgetstr(this->wCommandLine, str);
        this->redraw();
        if (strcmp(str, "quit") == 0) {
            break;
        }
    }
}

void
ALI::redraw()
{
    // output registers
    box(this->wRegisters, 0, 0);
    LC4::registers regs(this->lc4->get_registers());
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
    mvwprintw(this->wMemory, 1, 1, "0000: 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000");

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

    // print command line region
    wclear(this->wCommandLine);
    box(this->wCommandLine, 0, 0);
    mvwprintw(this->wCommandLine, 16, 1, "> ");

    // refresh windows
    wrefresh(wRegisters);
    wrefresh(wMemory);
    wrefresh(wCommandLine);

    // position cursor to correct location
    wmove(this->wCommandLine, 16, 3);

    refresh();
}

int main(int argc, char* argv[])
{
    LC4Machine lc4("test.bin");
    ALI ali(&lc4);

    ali.loop();

    /*
    mvwprintw(reg_windows, 1, 1, "R1=%08x", regs.r[0]);
    */

    return 0;
}
