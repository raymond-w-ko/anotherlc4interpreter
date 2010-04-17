#pragma once
#include <stdlib>
#include <stdio>
#include <iostream>
#include <fstream>
#include <string>

public class LC4Machine
{
  private:
        registers reg;
        string input_file, current_instruc;
        char* instruc_buff;
        bool is_binary;
        ifstream file;
        
        //initializes the machine
        void init();
        /**
        * Steps the LC4 Machine through one instruction
        * @return true if no errors have occurred, false 
        * if an error has occurred
        */
        bool step(); 
        
        /**
        * Parses the current instruction to determine what kind
        * of instruction it is
        * @param instruction the current instruction from file
        * @return the instruction code (ALU, BOOL, COMPARE, SHIFT, etc)
        */
        unsigned int parse_instr(string instruction);
        
        /**
        * Performs an ALU instruction
        * @param operation the opcode parsed from the instruction
        * @param dst the destination register for storing the result of the op
        * @param src the source register we will perform the operation on
        * @param t the register we will manipulate in conjunction with src OR
        *          the immediate value (as in and ADDI instruction)
        * @return the result that was stored to dst (should be used for debug
        *         purposes only)
        */
        unsigned int perform_ALU(unsigned short operation, 
                                 short dst, short src, short t);
        
        /**
         * Performs a Boolean instruction
         * @param operation the opcode parsed from the instruction
         * @param dst the destination register for storing the result of the op
         * @param src the source register we will perform the operation on
         * @param t the register we will manipulate in conjunction with src OR
         *          the immediate value (as in and ADDI instruction)
         * @return the result that was stored to dst (should be used for debug
         *         purposes only)
         */
        unsigned int perform_BOOL(unsigned short operations, 
                                  short dst, short src, short t);
        
  public:
    LC4Machine();
    LC4Machine(string filename);
};
