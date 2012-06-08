#include "MyHeaders.h"
#include <signal.h>
#include <time.h>
#include "MyLog.h"
using namespace std;

#define N 5000




int main()
{
  MYSQL mysql;
  mysql_init(&mysql);
  if(!mysql_real_connect(&mysql, "localhost", "chenmin", "chenmin", "sina", 3306, NULL, 0))
    {
      printf("connect error\n");
    }
  
  mysql_close(&mysql);
}
