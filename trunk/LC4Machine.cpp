#include "LC4Machine.h"
using namespace std;

void LC4Machine::init()
{
  if(input_file.empty())
  {
    cout<<"ERROR: LC4 Machine not initialized!"<<endl;
    cout<<"ERROR: No input provided!"<<endl;
    return;
  }
  
  file = new ifstream(input_file.c_str());
  
  if(!file.is_open())
  {
    cout<<"ERROR: LC4 Machine not initialized!"<<endl;
    cout<<"ERROR: " 
        <<input_file
        <<" does not exist!" 
        <<endl;
    return;
  }
}

bool LC4Machine::step()
{
  //load instruction
  if(is_binary)
  {
    //TODO binary logic
  }
  else
  {
    char* buff = (char*)calloc(17, sizeof(char));
    file.read(buff, 16);
    buff[16] = '\0'; //null terminator
    string instruc(buff);
    
    parse_instruc(instruc);
