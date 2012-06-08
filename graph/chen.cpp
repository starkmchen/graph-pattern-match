#include <iostream>
#include <string>
#include <sstream>
#include <string.h>
#include <stdio.h>
using namespace std;



int main()
{
  char s[]="sdjkf sdf dsf sdf sdf s";
  const char d[]=" ";
  char *p;
  p=strtok(s, d);
  while(p)
    {
      printf("%s\n", p);
      p=strtok(NULL, d);
    }
}
