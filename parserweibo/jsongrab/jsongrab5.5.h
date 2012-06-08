#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <mysql/mysql.h>
#include <sstream>
#include <dirent.h>
#include <time.h>
#include "json/json.h"
#include <stdlib.h>
#include <queue>
#include <pthread.h>
#include "../MyLog.h"

using namespace std;

struct token
{
    string type;
    string name;
    int level;
};
 
struct weiboinfo
{
  string mblogid;
  long basicinfo_uid;
  long basicinfo_time;
  long basicinfo_source;
  int basicinfo_rtnum;
  int basicinfo_cmtnum;
  string atusers;
  long rtinfo_rootuid;
  long rtinfo_fromuid;
  string rtinfo_fromid;
  string rtinfo_rootid;
  int rtinfo_rootrtnum;
  string rtinfo_rtreason;
  string basicinfo_topic;
  string filename;
  int lastitem;
};
 
struct weibo
{
    string content;
};
 
void getTokenFromConfig(vector<struct token> &vtoken, string &database, string &password, string &oldtable);
int itoa(int val, char* buf);

int jsongrabmysql(string filename);


