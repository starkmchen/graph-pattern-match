#include "graph.h"


adjlist_graph::adjlist_graph(int n):distance(NULL)
{
  initGraph(n);
}

int adjlist_graph::initGraph(int n)
{
  nm = n;
  vector<edge>v;
  set<string>w;

  for(int i=0; i<n; i++)
    {
      adjlist.push_back(v);
      vlabel.push_back(w);
    }
  return 0;
}


int adjlist_graph::addEdge(int s, int t, int bd)
{
  edge edg;
  edg.t=t;
  edg.bd=bd;
  adjlist[s].push_back(edg);
  return 0;
}


int adjlist_graph::labelVertex(int v, string &label)
{
  vlabel[v].insert(label);
  return 0;
}


int adjlist_graph::allDistance()
{
  int i, j;

  distance=new int*[nm];
  for(i=0; i<nm; i++)
    {
      distance[i]=new int[nm];
      for(j=0; j<nm; j++)
	{
	  distance[i][j]=-1;
	}
    }
  queue<int>tmpq;
  int nod;
  for(i=0; i<nm; i++)
    {
      tmpq.push(i);
      distance[i][i]=0;
      while(!tmpq.empty())
	{
	  nod=tmpq.front();
	  tmpq.pop();
	  for(j=0; j<adjlist[nod].size(); j++)
	    {
	      if(distance[i][adjlist[nod][j].t] == -1)
		{
		  tmpq.push(adjlist[nod][j].t);
		  distance[i][adjlist[nod][j].t]=distance[i][nod]+1;
		}
	    }
	}
    }
  return 0;
}


adjlist_graph::~adjlist_graph()
{
  int i;
  vlabel.clear();
  adjlist.clear();
  for(i=0; i<nm; i++)
    {
      if(distance != NULL)
	delete[] distance[i];
    }
  if(distance != NULL)
    {
      delete[] distance;
    }
}


adjmatrix_graph::adjmatrix_graph(int n)
{
  initGraph(n);
}


int adjmatrix_graph::initGraph(int n)
{
  int i, j;
  nm = n;
  set<string>w;

  adjmatrix=new int*[n];
  for(int i=0; i<n; i++)
    {
      vlabel.push_back(w);
      adjmatrix[i]=new int[n];
    }
  for(i=0; i<n; i++)
    {
      for(j=0; j<n; j++)
	adjmatrix[i][j]=-2;
    }
  return 0;
}

int adjmatrix_graph::addEdge(int s, int t, int bd)
{
  adjmatrix[s][t]=bd;
  return 0;
}

int adjmatrix_graph::labelVertex(int v, string &label)
{
  vlabel[v].insert(label);
  return 0;
}

adjmatrix_graph::~adjmatrix_graph()
{
  vlabel.clear();
  for(int i=0; i<nm; i++)
    {
      delete[] adjmatrix[i];
    }
  delete[] adjmatrix;
}

