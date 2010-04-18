﻿CFLAGS=-Wall -lcurses

all: lc4

lc4: *.cpp *.h Makefile
	g++ $(CFLAGS) -o lc4 ali.cpp LC4Machine.cpp ALU.cpp compare.cpp JSR.cpp bitwise.cpp barrel_roller.cpp JMP.cpp memory.cpp branch.cpp
