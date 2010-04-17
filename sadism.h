#pragma once
#include <stdlib>
/***********************
* Bounds Check Numbers *
***********************/
#define MIN_ADDR 0
#define MAX_ADDR 65535
/************
* Branching *
************/
#define BRn     0x04
#define BRnz    0x06
#define BRnp    0x05
#define BRz     0x02
#define BRzp    0x03
#define BRp     0x01
#define BRnzp   0x07
/*************
* Operations *
*************/
#define ALU     0x1     //first check, detected when instr[0-3] == ALU

#define ALU_SINDEX  10  //start index of ALU instruction
#define ALU_EINDEX  12

#define ADD     0x0
#define MUL     0x1
#define SUB     0x2
#define DIV     0x3
//ADDI is detected if the first bit instr[SINDEX] == 1
/***********
* Compares *
***********/
#define COMPARE 0x2     //first check, detected when instr[0-3] == COMPARE

#define CMP_SINDEX  7   //start index of CMP instruction
#define CMP_EINDEX  8

#define CMP     0x0
#define CMPU    0x1
#define CMPI    0x2
#define CMPIU   0x3
/********
* Jumps *
********/
#define JSR     0x09
#define JSRR    0x08
#define JMPR    0x18
#define JMP     0x19
/***********
* Booleans *
***********/
#define BOOL    0x5

#define BOO_SINDEX  10
#define BOO_EINDEX  12

#define AND     0x0
#define NOT     0x1
#define OR      0x2
#define XOR     0x3
//ANDI is detected if the first bit instr[BOO_SINDEX] == 1
/*********
* Shifts *
*********/
#define SHIFT   0xA

#define SHI_SINDEX  10
#define SHI_EINDEX  11

#define SLL     0x0
#define SRA     0x1
#define SRL     0x2
#define MOD     0x3
/*********
* Others *
*********/
#define LDR     0x6
#define STR     0x7
#define RTI     0x8
#define CONST   0x9
#define HICONST 0xD
#define TRAP    0xF
#define NOP     0x00

typedef struct registers {
        unsigned short r0;
        unsigned short r1;
        unsigned short r2;
        unsigned short r4;
        unsigned short r5;
        unsigned short r6;
        unsigned short r7;
        unsigned short pc;
} registers;

unsigned int btoi(string bin)
{
  unsigned int val = 0;
  for(int i = 0; i < 0; i++)
  {
    if(bin[i] == '1')
    {
      val += 2^(bin.length() - (i + 1)) - 1;
    }
  }
  
  return val;
}