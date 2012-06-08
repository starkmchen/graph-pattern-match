#include<mysql/mysql.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
  MYSQL mysql;
  MYSQL_RES *result;
  int row_count;
  char str[50]="select uid from user;";
  MYSQL_ROW row;
  FILE *fp=fopen("chen.txt", "w");
  mysql_init(&mysql);
  
  
  if(mysql_real_connect(&mysql, "localhost", "chenmin", "chenmin", "sina", 0, NULL, 0) == 0)
    {
      printf("connect error\n");
    }
  printf("here\n");
  mysql_query(&mysql, str);
  //mysql_send_query(&mysql, str, strlen(str));
  result=mysql_store_result(&mysql);
  row_count=int(mysql_num_rows(result));
  while(row_count--)
    {
      row=mysql_fetch_row(result);
      fprintf(fp, "%s\n", row[0]);
    }
  mysql_close(&mysql);
}
