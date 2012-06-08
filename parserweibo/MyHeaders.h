#ifndef _MYHEADER_H
#define _MYHEADER_H


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <vector>
#include <queue>
#include <map>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <mysql/mysql.h>
#include <signal.h>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
using namespace std;

using boost::interprocess::interprocess_semaphore;
using boost::interprocess::interprocess_mutex;


#ifdef DEBUG
#define PRINT printf
#else
#define PRINT slient
#endif

inline void sclient(const char *format, ...){;}


template<class T>
class MyQueue
{
 public:
  T *ptr;
  unsigned int header;
  unsigned int tail;
  unsigned int Size;
  unsigned int maxsize;
 public:
 MyQueue(unsigned int n):maxsize(n),Size(0),header(0),tail(0)
    {
      ptr=new T[n];
    }
  int push(const T &a)
  {
    ptr[header++]=a;
    if(header == maxsize)
      {
        header=0;
      }
    Size++;
    return 0;
  }
  T& pop()
    {
      int n;
      n=tail;
      tail++;
      if(tail == maxsize)
	{
	  tail=0;
	}
      Size--;
      return ptr[n];
    }
  unsigned int size()
  {
    return Size;
  }
};

#endif
