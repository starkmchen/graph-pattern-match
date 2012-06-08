#ifndef __MYSTR_H_
#define __MYSTR_H_
#include <string.h>
#include <string>
using std::string;

class MyStr
{
  char str[32];
  int Size;
public:
 MyStr():Size(0){;}
  MyStr(const char *s)
    {
      init(s);
    }
  MyStr(const string &s)
    {
      init(s);
    }
  bool init(const char *s)
    {
      if((Size =strlen(s)) >= 32)
	{
	  Size=0;
	  return 0;
	}
      strcpy(str, s);
      return 1;
    }
  bool init(const string &s)
  {
    if((Size=s.size()) >= 32)
      {
	Size=0;
	return 0;
      }
    strcpy(str, s.c_str());
    return 1;
  }
  bool operator==(const MyStr &ano)
  {
    return strcmp(ano.str, str) == 0;
  }
  unsigned int hashFunc()
  {
    unsigned int hash = 0;
    char *s=str;
    while (*s)
      {
        hash = (*s++) + (hash << 6) + (hash << 16) - hash;
      }
 
    return (hash & 0x7FFFFFFF) % 0x10000;
  }
};

#endif
