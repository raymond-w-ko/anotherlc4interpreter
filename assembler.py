#!/usr/bin/python
import sys
from array import array

reg0 = "000"
reg1 = "001"
reg2 = "010"
reg3 = "011"
reg4 = "100"
reg5 = "101"
reg6 = "110"
reg7 = "111"

def sanitize(instruc, type = 0):
  if type == 0:
    instruc = instruc.replace(","," ")
    instruc = instruc.lower()
    instruc = instruc.split(" ") #instruc is now a list
  
  for element in instruc:
    if element.find(" ") != -1:
      element.replace(" ", "")
    if element == "":
      instruc.remove(element)
  
  return instruc

def isInt(val):
  ok = 1
  try:
    num = int(val)
  except ValueError:
    ok = 0
  
  return ok
  
def isHex(val):
  return val[0:2] == "0x"

def immToInt(imm):
  if isHex(imm):
    imm = int(imm[2:], 16)
  elif isInt(imm):
    imm = int(imm)
  else:
    print "ERROR: " + imm + " is not a number. WTF Dude"
  
  return imm

def reg2bin(register):
  register = register.lower()
  if register == "r0":
    return reg0
  elif register == "r1":
    return reg1
  elif register == "r2":
    return reg2
  elif register == "r3":
    return reg3
  elif register == "r4":
    return reg4
  elif register == "r5":
    return reg5
  elif register == "r6":
    return reg6
  elif register == "r7":
    return reg7
  else:
    print "ERROR: " + register + " is NOT a proper register."
    print "Thou shalt use registers r0, r1, r2, r3, r4, r5, r6, and r7"
    exit(0)

def int2bin(n, count=16):
    #returns the binary of integer n, using count number of digits
    return "".join([str((n >> y) & 1) for y in range(count-1, -1, -1)])

def parse_branch(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  ins_list = instruc.split(" ")
  ins_list = sanitize(ins_list)
  ins = ins_list[0]
  ins = ins.lower()
  imm = ins_list[1]
  imm = immToInt(imm)
  if imm > (2**8) or imm < -(2**8) + 1:
    print "ERROR: Immediate value " + str(imm) +" is out of range."
    print "IMM in BR(nzp) IMM must be greater than -(2**8) + 1 and less than 2**8"
    return "-1"
  #get sign correct on the binary
  sign = "0"
  if(imm < 0):
    imm = 2**10 - 2 + imm
  
  imm = int2bin(imm, 9)
  
  
  if ins == "brn":
    word_buf = word_buf + "100"
  elif ins == "brnz":
    word_buf = word_buf + "110"
  elif ins == "brnp":
    word_buf = word_buf + "101"
  elif ins == "brz":
    word_buf = word_buf + "010"
  elif ins == "brzp":
    word_buf = word_buf + "011"
  elif ins == "brp":
    word_buf = word_buf + "001"
  elif ins == "brnzp":
    word_buf = word_buf + "111"
  else:
    print "Someone accidentally'd a branch"
    print "ERROR: " + instruc + " is an invalid branch instruction"
    return "-1"
  
  return word_buf + imm
  
  
def parse_alu(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_alu"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regD = instruc[1]
  regS = instruc[2]
  regT = instruc[3]
  regD = reg2bin(regD)
  regS = reg2bin(regS)
  word_buf += regD
  word_buf += regS
  
  if ins == "add":
    if isInt(regT) or isHex(regT):
      word_buf += "1"
      regT = immToInt(regT)
      word_buf += int2bin(regT, 5)
    else:
      word_buf += "000"
      word_buf += reg2bin(regT)
  elif ins == "mul":
    word_buf += "001"
    word_buf += reg2bin(regT)
  elif ins == "sub":
    word_buf += "010"
    word_buf += reg2bin(regT)
  elif ins == "div":
    word_buf += "011"
    word_buf += reg2bin(regT)
  else:
    print "ERROR: someone accidentally'd the parse_alu method"
    return "-1"
  
  return word_buf
  

def parse_compare(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_compare"
    return "-1"
  
  blank = "0000"
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regS = instruc[1]
  regT = instruc[2]
  
  word_buf += reg2bin(regS)
  if ins == "cmp":
    word_buf += "00"
  elif ins == "cmpu":
    word_buf += "01"
  elif ins == "cmpi":
    word_buf += "10"
    imm = immToInt(regT)
    if(imm < 0):
      imm = 2**8 - 2 + imm
      imm = int2bin(imm, 7)
    else:
      imm = int2bin(imm, 7)
    
    word_buf += imm
  elif ins == "cmpiu":
    word_buf += "11"
    imm = int(regT)
    imm = int2bin(imm, 7)
    word_buf += imm
  else:
    print "Someone accidentally'd the code in parse_compare"
  
  return word_buf

def parse_jump(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_jump"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regS = instruc[1]
  
  if ins == "jsr":
    word_buf += "1"
    imm = immToInt(regS)
    if imm > 2**10 or imm < -(2**10) + 1:
      print "ERROR: Immediate value " + str(imm) +" is out of range."
      print "IMM in JSR IMM must be greater than -(2**10) + 1 and less than 2**10"
      return "-1"
    
    if imm < 0:
      imm = 2**12 - 2 + imm
      imm = int2bin(imm, 11)
    else:
      imm = int2bin(imm, 11)
    
    word_buf += imm
    
  elif ins == "jsrr":
    word_buf += "0"
    word_buf += "00"
    word_buf += reg2bin(regS)
    word_buf += "000000"
  
  elif ins == "jmpr":
    word_buf += "0"
    word_buf += "00"
    word_buf += reg2bin(regS)
    word_buf += "000000"
  
  elif ins == "jmp":
    word_buf += "1"
    imm = immToInt(regS)
    if imm > 2**10 or imm < -(2**10) + 1:
      print "ERROR: Immediate value " + str(imm) +" is out of range."
      print "IMM in JMP IMM must be greater than -(2**10) + 1 and less than 2**10"
      return "-1"
    
    if imm < 0:
      imm = 2**12 - 2 + imm
      imm = int2bin(imm, 11)
    else:
      imm = int2bin(imm, 11)
    
    word_buf += imm
    
  else:
    print "ERROR: Someone accidentally'd the code in parse_jump"
    return "-1"
  
  return word_buf

def parse_bool(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_bool"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regD = instruc[1]
  regS = instruc[2]
  regT = ""
  if ins != "not":
    regT = instruc[3]
  
  if ins == "and":
    if isInt(regT) or isHex(regT):
      word_buf += reg2bin(regD)
      word_buf += reg2bin(regS)
      word_buf += "1"
      imm = immToInt(regT)
      if imm > 2**4 or imm < (-2**4 + 1):
        print "ERROR: Immediate value " + str(imm) +" is out of range."
        print "IMM in AND Dst, Src, IMM must be greater than -(2**4) + 1 and less than 2**4"
        return "-1"
      else:
        if imm < 0:
          imm = 2**6 - 2 + imm
          imm = "1" + int2bin(imm, 5)
        else:
          imm = int2bin(imm, 5)
        
        word_buf += imm
    else: #is a register
      word_buf += reg2bin(regD)
      word_buf += reg2bin(regS)
      word_buf += "000"
      word_buf += reg2bin(regT)
    
  elif ins == "not":
    word_buf += reg2bin(regD)
    word_buf += reg2bin(regS)
    word_buf += "001"
    word_buf += "000"
  
  elif ins == "or":
    word_buf += reg2bin(regD)
    word_buf += reg2bin(regS)
    word_buf += "010"
    word_buf += reg2bin(regT)
  
  elif ins == "xor":
    word_buf += reg2bin(regD)
    word_buf += reg2bin(regS)
    word_buf += "011"
    word_buf += reg2bin(regT)
  else:
    print "ERROR: someone accidentally the code in parse_bool"
    return "-1"
  
  return word_buf

def parse_mem(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_mem"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regD = instruc[1]
  regS = instruc[2]
  imm = instruc[3]
  imm = immToInt(imm)
  if imm > 2**5 or imm < -(2**5) + 1:
    print "ERROR: Immediate value " + str(imm) +" is out of range."
    print "IMM in LDR/STR Dst, Src, IMM must be greater than -(2**5) + 1 and less than 2**5"
    return "-1"
  
  if imm < 0:
    imm = 2**7 + imm  
    imm = int2bin(imm, 6)
  else:
    imm = int2bin(imm, 6)
  
  if ins == "ldr":
    word_buf += "0"
  elif ins == "str":
    word_buf += "1"
  else:
    print "ERROR: someone accidentally'd the parse_mem method"
    return -1
  
  word_buf += reg2bin(regD)
  word_buf += reg2bin(regS)
  word_buf += imm
  
  return word_buf

def parse_const(instruc = "", word_buf = ""):
  if instruc == "":
    print "ERROR: someone accidentally the code in parse_const"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regD = instruc[1]
  imm = instruc[2]
  imm = immToInt(imm)
  
  if ins == "const":
    if imm > 2**8 or imm < -(2**8) + 1:
      print "ERROR: Immediate value " + str(imm) +" is out of range."
      print "IMM in CONST Dst, IMM must be greater than -(2**8) + 1 and less than 2**8"
      return "-1"
    
    if(imm < 0):
      imm = 2**9 - 2 + imm
      imm = int2bin(imm, 9)
    else:
      imm = int2bin(imm, 9)
    
    word_buf += "1001"
    word_buf += reg2bin(regD)
    word_buf += imm
  
  elif ins == "hiconst":
    if imm > 2**8 or imm < 0:
      print "ERROR: Immediate value " + str(imm) +" is out of range."
      print "IMM in HICONST Dst UIMM must be greater than 0 and less than 2**8"
      return "-1"
    
    word_buf += "1101"
    word_buf += reg2bin(regD)
    word_buf += "1"
    word_buf += int2bin(imm, 8)
  
  else:
    print "ERROR: someone accidentally'd the parse_const method"
    return "-1"
  
  return word_buf

def parse_shift(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_shift"
    return "-1"
  
  instruc = sanitize(instruc)
  
  ins = instruc[0]
  regD = instruc[1]
  regS = instruc[2]
  regT = instruc[3]
  imm = regT
  if isInt(imm) or isHex(imm):
    imm = immToInt(imm)
    if imm > 2**4 or imm < 0:
      print "ERROR: Immediate value " + str(imm) +" is out of range."
      print "IMM in SLL/SRA/SRL Dst, Src, UIMM must be greater than 0 and less than 2**4"
      return "-1"
  
  word_buf += reg2bin(regD)
  word_buf += reg2bin(regS)
  
  if ins == "sll":
    word_buf += "00"
    word_buf += int2bin(imm, 4)
  elif ins == "sra":
    word_buf += "01"
    word_buf += int2bin(imm, 4)
  elif ins == "srl":
    word_buf += "10"
    word_buf += int2bin(imm, 4)
  elif ins == "mod":
    word_buf += "11"
    word_buf += "0"
    word_buf += reg2bin(regT)
  else:
    print "ERROR: someone accidentally'd the parse_shift method"
  
  return word_buf

def parse_trap(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_trap"
    return "-1"
  
  instruc = sanitize(instruc)
  
  imm = instruc[1]
  imm = immToInt(imm)
  if imm > 2**8 or imm < 0:
    print "ERROR: Immediate value " + str(imm) +" is out of range."
    print "IMM in TRAP UIMM must be greater than 0 and less than 2**8"
    return "-1"
  
  word_buf += "0000"
  word_buf += int2bin(imm, 8)
  
  return word_buf

def parse(instruc = ""):
  word_buf = ""
  instruc = instruc.strip()
  if instruc.lower() == "ret":
    instruc = "jmpr r7"
  instruc_list = instruc.split(" ")
  instruc_list = sanitize(instruc_list, 1)
  
  word = instruc_list[0]
  #for word in instruc:
  ins = word.lower()
  if ins == "nop":
    return "0000000000000000"
  
  if (ins == "brn" or
  ins == "brnz" or
  ins == "brnp" or
  ins == "brz" or
  ins == "brzp" or
  ins == "brp" or
  ins == "brnzp"):
    word_buf += "0000"
    word_buf = parse_branch(instruc, word_buf)
    return word_buf
  
  if (ins == "add" or 
  ins == "mul" or 
  ins == "sub" or
  ins == "div"):
    word_buf += "0001"
    word_buf = parse_alu(instruc, word_buf)
    return word_buf
	
  if (ins == "cmp" or
  ins == "cmpu" or
  ins == "cmpi" or
  ins == "cmpiu"):
    word_buf += "0010"
    word_buf = parse_compare(instruc, word_buf)
    return word_buf
    
  if (ins == "jsr" or
  ins == "jsrr"):
    word_buf += "0100"
    word_buf = parse_jump(instruc, word_buf)
    return word_buf
  
  if (ins == "jmp" or
  ins == "jmpr"):
    word_buf += "1100"
    word_buf = parse_jump(instruc, word_buf)
    return word_buf
  
  if (ins == "and" or
  ins == "not" or
  ins == "or" or
  ins == "xor"):
    word_buf += "0101"
    word_buf = parse_bool(intruc, word_buf)
    return word_buf
  
  if ins == "rti":
    word_buf += "1000000000000000"
    return word_buf
  
  if (ins == "ldr" or
  ins == "str"):
    word_buf += "011"
    word_buf = parse_mem(instruc, word_buf)
    return word_buf
  
  if (ins == "const" or
  ins == "hiconst"):
    word_buf = parse_const(instruc, word_buf)
    return word_buf
  
  if (ins == "sll" or
  ins == "sra" or
  ins == "srl" or
  ins == "mod"):
    word_buf += "1010"
    word_buf = parse_shift(instruc, word_buf)
    return word_buf
  
  if ins == "trap":
    word_buf += "1111"
    word_buf = parse_trap(instruc, word_buf)
    return word_buf
  
  print instruc + " is not a valid LC4 command"
  print "ERROR: Assembler error @ following instruction: " + instruc
  return "-1"

def main():
  #get filename from commandline
  filename = ""
  try:
    filename = sys.argv[1]
  except IOError:
    print filename + " does not exist. Please don't make me try and open it."
    sys.exit(0)
  
  if (filename == ""):
    print "Please provide a target file for the assembler to assemble"
    sys.exit(0)
  
  file = open(filename, "r")
  output_file = open(filename + ".bin", "wb")
  
  for line in file:
    binary = parse(line)
    if binary != "-1":
      print binary
      B = int(binary[8:16],2)
      A = int(binary[0:8],2)
      binary = array('H', [A + B*256])
      binary.tofile(output_file)
    else:
      print "An error has occurred. Please check the previous errors to determine where you went wrong, you bad, bad boy."
      output_file.truncate(0)
      output_file.write("Something went wrong during compilation... Your fault, not mine")
      break
  
  file.close()
  output_file.close()
  exit(0)

if __name__ == "__main__":
  main()
