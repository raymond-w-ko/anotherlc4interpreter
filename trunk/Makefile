CFLAGS=-Wall -lncurses -ggdb3

all: ali

ali: *.cpp *.h Makefile
	g++ -z execstack $(CFLAGS) -o ali ali.cpp LC4Machine.cpp ALU.cpp compare.cpp JSR.cpp bitwise.cpp barrel_roller.cpp JMP.cpp memory.cpp branch.cpp main.cpp

clean:
	rm -f ali
