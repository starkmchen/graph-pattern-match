#include "graph.h"
#include<iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <map>
#include<unistd.h>
#include<sys/time.h>
#include<protocol/TBinaryProtocol.h>
#include<transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include<Hbase.h>
#include <string.h>

using namespace std;
using namespace apache::hadoop::hbase::thrift;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;

#define PATGRAPH "PatGraph"
#define DATGRAPH "DatGraphInfo"


FILE *fp=freopen("data.txt", "r", stdin);

shared_ptr<TTransport> hbasesocket(new TSocket("192.168.1.163", 9090));
shared_ptr<TTransport> hbasetransport(new TBufferedTransport(hbasesocket));
shared_ptr<TProtocol> hbaseprotocol(new TBinaryProtocol(hbasetransport));
HbaseClient hbaseclient(hbaseprotocol);


int patn;
int datn;


vector<int>disv1;;
vector<int>disv2;

bool intersect(set<int>&s1, set<int>&s2)
{
  set<int>::iterator ite1, ite2;
  ite1=s1.begin();
  ite2=s2.begin();
  while(1)
    {
      if(ite1 == s1.end() || ite2 == s2.end())
        break;
      if(*ite1 < *ite2)
        ite1++;
      else if(*ite1 > *ite2)
        ite2++;
      else
        return 1;
    }
  return 0;
}


int labelMatch(set<string>&pat, set<string>&dat)
{
  set<string>::iterator ite;
  for(ite=pat.begin(); ite!=pat.end(); ite++)
    {
      if(!dat.count(*ite))
	{
	  return 0;
	}
    }
  return 1;
}



int labelMatchMap(int pn, int dn)
{
  set<string>s1;
  set<string>s2;

  vector<TRowResult>result;
  vector<string>cols;
  vector<Mutation>v;

  Mutation muta;
  char row[32];
  char colname[32];
  string pdeg, ddeg;
  string plabel, dlabel;
  char *p;
  char delim[]=" ";
  char tmplabel[64];

  sprintf(row, "%d", pn);
  cols.push_back("outdeg:");
  cols.push_back("label:");
  hbaseclient.getRowWithColumns(result, PATGRAPH, row, cols);
  pdeg=result[0].columns["outdeg:"].value;
  sprintf(tmplabel, "%s", result[0].columns["label:"].value.c_str());
  p=strtok(tmplabel, delim);
  while(p)
    {
      s1.insert(p);
      p=strtok(NULL, delim);
    }

  result.clear();
  sprintf(row, "v%d", dn);
  hbaseclient.getRowWithColumns(result, DATGRAPH, row, cols);
  ddeg=result[0].columns["outdeg:"].value;
  sprintf(tmplabel, "%s", result[0].columns["label:"].value.c_str());
  p=strtok(tmplabel, delim);
  while(p)
    {
      s2.insert(p);
      p=strtok(NULL, delim);
    }
  
  if((pdeg != "0" && ddeg == "0") || !labelMatch(s1, s2))
    return 0;

  sprintf(row, "labelmatch%d", pn);
  sprintf(colname, "vertexs:%d",dn);
  muta.column=colname;
  muta.value="1";
  v.push_back(muta);
  hbaseclient.mutateRow(DATGRAPH, row, v);
  
}


int getLabelMatch()
{
  int i, j;
  
  for(i=0; i<patn; i++)
    {
      for(j=0; j<datn; j++)
	{
	  labelMatchMap(i, j);
	}
    }
  return 0;
}



void initPatGraph(int n)
{
  int i,j;
  Mutation muta;
  vector<BatchMutation>bms;

  char row[8];
  char colname[32];
  char value[8];

  BatchMutation bm;

  muta.isDelete=0;
  muta.column="vn";
  sprintf(value, "%d", n);
  muta.value=value;
  bm.row="graph";
  bm.mutations.push_back(muta);
  bms.push_back(bm);

  for(i=0; i<n; i++)
    {
      sprintf(row, "%d",i);
      bm.row=row;
      
      muta.column="outdeg";
      muta.value="0";
      bm.mutations.push_back(muta);

      for(j=0; j<n; j++)
	{
	  sprintf(colname, "vertexs:%d", j);
	  muta.column=colname;
	  muta.value="-2";
	  bm.mutations.push_back(muta);
	}
      bms.push_back(bm);
      bm.mutations.clear();
    }
  hbaseclient.mutateRows(PATGRAPH, bms);
}

void initGraph()
{
  int n, i, s, t, bd, j;
  string line;

  char row[32];
  char value[32];
  char colname[32];
  ColumnDescriptor col;
  vector<ColumnDescriptor>cols;

  vector<BatchMutation>bms;
  BatchMutation bm;
  Mutation muta;
  
  try
    {
      hbaseclient.disableTable(PATGRAPH);
      hbaseclient.deleteTable(PATGRAPH);
    }
  catch(IOError &io)
    {
      printf("drop patgraph\n");
    }
  try
    {
      hbaseclient.disableTable(DATGRAPH);
      hbaseclient.deleteTable(DATGRAPH);
    }
  catch(IOError &io)
    {
      printf("drop datgraph\n");
    }
  
  col.name="vertexs";
  cols.push_back(col);
  col.name="vn";
  cols.push_back(col);
  col.name="label";
  cols.push_back(col);
  col.name="outdeg";
  cols.push_back(col);
  hbaseclient.createTable(PATGRAPH, cols);
  hbaseclient.createTable(DATGRAPH, cols);

  cin >> n;
  patn=n;
  initPatGraph(n);
  
  getchar();
  muta.isDelete=0;

  for(i=0; i<patn; i++)
    {
      sprintf(row, "%d", i);
      bm.row=row;

      getline(cin, line);
      
      muta.column="label";
      muta.value=line;
      bm.mutations.push_back(muta);
      
      bms.push_back(bm);
      bm.mutations.clear();
    }
  
  cin >> n;
  for(i=0; i<n; i++)
    {
      cin >> s >> t >> bd;

      muta.column="outdeg";
      muta.value="1";
      bms[s].mutations.push_back(muta);

      sprintf(colname, "vertexs:%d",t);
      muta.column=colname;
      sprintf(value, "%d", bd);
      muta.value=value;
      bms[s].mutations.push_back(muta);
    }
  hbaseclient.mutateRows(PATGRAPH, bms);
  bms.clear();
  
  cin >> n;
  datn = n;

  for(i=0; i<patn; i++)
    {
      sprintf(row, "labelmatch%d", i);
      bm.row=row;
      for(j=0; j<datn; j++)
	{
	  sprintf(colname, "vertexs:%d", j);
	  muta.column=colname;
	  muta.value="0";
	  bm.mutations.push_back(muta);
	}
      hbaseclient.mutateRow(DATGRAPH, bm.row, bm.mutations);
      bm.mutations.clear();
    }

  getchar();

  for(i=0; i<datn; i++)
    {
      sprintf(row, "v%d", i);
      bm.row=row;
      
      getline(cin, line);

      muta.column="label";
      muta.value=line;
      bm.mutations.push_back(muta);
      
      hbaseclient.mutateRow(DATGRAPH,bm.row, bm.mutations);
      bm.mutations.clear();
    }

  cin >> n;
  for(i=0; i<n; i++)
    {
      cin >> s >> t >> bd;

      sprintf(row, "v%d", s);
      bm.row=row;
      sprintf(colname, "vertexs:%d", t);
      muta.column=colname;
      muta.value="1";
      bm.mutations.push_back(muta);
      hbaseclient.mutateRow(DATGRAPH, bm.row, bm.mutations);
      bm.mutations.clear();
    }
  for(i=0; i<datn; i++)
    {
      for(j=0; j<datn; j++)
	{
	  sprintf(row, "d%d", i);
	  bm.row=row;
	  sprintf(colname, "vertexs:%d", j);
	  muta.column=colname;
	  muta.value=-1;
	  bm.mutations.push_back(muta);
	}
      hbaseclient.mutateRow(DATGRAPH, bm.row, bm.mutations);
      bm.mutations.clear();
    }
  getLabelMatch();

}


void bfsvertex()
{
  
}


int datAllDis()
{
  int i;
  for(i=0; i<datn; i++)
    {
      disv1.push_back(i);
      bfsvertex();
    }
}



int main()
{
  
  hbasetransport->open();

  initGraph();
  datAllDis();
  /*
  Datg.allDistance();
  if(!initAnc())
    {
      return 0;
    }
  patternMatchv0();
  */
  hbasetransport->close();
}
