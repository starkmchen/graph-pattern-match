#ifndef MY_GRAPH_H__
#define MY_GRAPH_H__
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include<string>
#include <queue>
#include <string.h>
#include<sstream>
using namespace std;

struct edge
{
  int t;
  int bd;
};

class adjmatrix_graph
{
  int nm;
 public:
  int **adjmatrix;
  vector< set<string> > vlabel;
 adjmatrix_graph():adjmatrix(NULL){;}
  adjmatrix_graph(int);
  int initGraph(int);
  int addEdge(int,int,int);
  int vertexNum(){return nm;}
  int labelVertex(int, string &);
  ~adjmatrix_graph();
};

class adjlist_graph
{
  int nm;
 public:
  vector< set<string> > vlabel;
  vector< vector<edge> >   adjlist;
  int **distance;
  adjlist_graph(int);
 adjlist_graph():distance(NULL){;}
  int initGraph(int);
  int addEdge(int,int,int);
  int vertexNum(){return nm;}
  int labelVertex(int, string &);
  int allDistance();
  ~adjlist_graph();
};


#endif
