#include "LC4Machine.h"
#include <ncurses.h>

int main(int argc, char* argv[])
{
    LC4Machine lc4("test.bin");

    // initialized ncurses
    initscr();

    // cleanup ncurses
    endwin();
    return 0;
}
