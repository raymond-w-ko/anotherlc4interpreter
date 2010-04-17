import sys

reg0 = "000"
reg1 = "001"
reg2 = "010"
reg3 = "011"
reg4 = "100"
reg5 = "101"
reg6 = "110"
reg7 = "111"

def int2bin(n, count=16):
    #returns the binary of integer n, using count number of digits
    return "".join([str((n >> y) & 1) for y in range(count-1, -1, -1)])

def parse_branch(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  ins_list = instruc.split(" ")
  ins = ins_list[0]
  ins = ins.lower()
  
  imm = ins_list[1]
  imm = int(imm, 10)
  if imm > (2^8) or imm < -(2^8 + 1):
    print "ERROR: Immediate value " + str(imm) +" is out of range."
    print "IMM in BR(nzp) IMM must be greater than -(2^8 + 1) and less than 2^8"
    return "-1"
  #get sign correct on the binary
  sign = "0"
  if(imm < 0):
    imm = imm * -1
    sign = "1"
  
  imm = int2bin(imm)
  imm = sign + imm
  
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
  
  instruc = instruc.replace(","," ")
  instruc = instruc.lower()
  instruc = instruc.split(" ") #instruc is now a list
  
  ins = instruc[0]
  
def parse_compare(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_compare"
    return "-1"
  
  #TODO

def parse_jump(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO
  
def parse_bool(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO
  
def parse_mem(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO

def parse_const(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO

def parse_shift(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO

def parse_trap(instruc = "", word_buf = ""):
  if instruc == "" or word_buf == "":
    print "ERROR: someone accidentally the code in parse_branch"
    return "-1"
  
  #TODO

def parse(instruc = ""):
  word_buf = ""
  if instruc.lower() == "ret":
    instruc = "jmpr r7"
  instruc_list = instruc.split(" ")
  word = instruc_list[0]
  #for word in instruc:
  ins = word.lower()
  print "Working on instruction " + ins
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
    word_buf += "1000"
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
  filename = sys.argv[1]
  
  if (filename == ""):
    print "Please provide a filename for the assembler to assemble"
    sys.exit(0)
  
  file = open(filename, "r")
  output_file = open(filename + ".asm", "w")
  
  for line in file:
    binary = parse(line)
    if binary != "-1":
      output_file.write(binary)
      output_file.write("\n")
  
  file.close()
  output_file.close()
  exit(0)

if __name__ == "__main__":
  main()