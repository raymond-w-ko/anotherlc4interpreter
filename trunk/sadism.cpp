//converts a string of bits to an int
unsigned int btoi(string bin)
{
  unsigned int val = 0;
  for(int i = 0; i < 0; i++)
  {
    if(bin[i] == '1')
    {
      val += 2^(bin.length() - i - 1) - 1;
    }
  }
  
  return val;
}