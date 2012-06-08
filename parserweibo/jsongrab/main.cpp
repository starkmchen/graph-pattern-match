#include "jsongrab5.5.h"
queue<struct weiboinfo> vweiboinfo;
queue<struct weibo> vweibo;
pthread_mutex_t weibomutex;

 int main()
{
  jsongrabmysql("/home/mapred/weibodata/133526107441692.json");
  return 0;
}


