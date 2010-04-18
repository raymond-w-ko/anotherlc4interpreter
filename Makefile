CFLAGS=-Wall -lcurses

all: ali

ali: *.cpp *.h Makefile
	g++ $(CFLAGS) -o ali ali.cpp LC4Machine.cpp ALU.cpp compare.cpp JSR.cpp bitwise.cpp barrel_roller.cpp JMP.cpp memory.cpp branch.cpp
