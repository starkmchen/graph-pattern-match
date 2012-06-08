#ifndef __MYLOG__H__
#define __MYLOG__H__
#include "MyHeaders.h"

class MyLog
{
  int itemnum;
  int itemmax;
  string logdir;
  string logprefix;
  FILE *fp;
  int addtime;
  string createFileName()
  {
    char tmpstr[32];
    string logname;
    time_t t;
    struct tm *tmp;
    logname += logdir;
    logname += logprefix;
    time(&t);
    tmp=localtime(&t);
    sprintf(tmpstr, "%02d-%02d-%02d-%02d-%02d", tmp->tm_mon, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
    logname += tmpstr;
    return logname;
  }

public:
  int initFile(string dir, string prefix)
  {
    logdir=dir;
    logprefix=prefix;
    fp=fopen(createFileName().c_str(), "w");
    if(fp == NULL)
      {
	return 1;
      }
    return 0;
  }


 MyLog(int n, int adt):itemnum(0),itemmax(n),addtime(adt)
    {
    };
 MyLog(int adt):itemnum(0),itemmax(-1),addtime(adt)
    {
    }
  int flush()
  {
    return fflush(fp);
  }
  int write(const char *format, ...)
  {
    va_list arg;
    int done;
    struct tm * tmp;
    time_t t;
    
    if(addtime)
      {
	time(&t);
	tmp=localtime(&t);
	fprintf(fp, "%02d-%02d-%02d-%02d-%02d: ", tmp->tm_mon, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
      }
    va_start(arg, format);
    done=vfprintf(fp, format, arg);
    va_end(arg);
    
    if(itemmax > 0)
      {
        itemnum++;
        if(itemnum == itemmax)
          {
            close();
            initFile(logdir, logprefix);
	    itemnum=0;
          }
      }
    return done;
  }
  int close()
  {
    if(fp != NULL)
      {
	fclose(fp);
      }
    return 0;
  }
  
};





#endif 


