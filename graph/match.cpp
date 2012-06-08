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

using namespace std;
using namespace apache::hadoop::hbase::thrift;
using namespace boost;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;


FILE *fp=freopen("data.txt", "r", stdin);

shared_ptr<TTransport> hbasesocket(new TSocket("192.168.1.163", 9090));
shared_ptr<TTransport> hbasetransport(new TBufferedTransport(hbasesocket));
shared_ptr<TProtocol> hbaseprotocol(new TBinaryProtocol(hbasetransport));
HbaseClient hbaseclient(hbaseprotocol);




struct node
{
  int ps;
  int pt;
  int gn;
  bool operator<(const node &ano)const
  {
    if(ps < ano.ps)
      return 1;
    else if(ps > ano.ps)
      return 0;
    else 
      {
	if(pt < ano.pt)
	  return 1;
	else if(pt > ano.pt)
	  return 0;
	else 
	  {
	    return gn < ano.gn;
	  }
      }
  }
};

adjmatrix_graph Patg;
adjlist_graph Datg;
int **lmatch;
set<int> *mat;
set<int> *matdel;
map<node, set<int> >anc;
map<node, set<int> >desc;

set<node> premv;

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

void initGraph()
{
  int n, i, s, t, bd, j, tmp, k, outdeg;
  string line, label;
  stringstream ssin;

  cin >> n;
  lmatch=new int*[n];
  mat=new set<int>[n];
  matdel=new set<int>[n];

  getchar();
  Patg.initGraph(n);
  for(i=0; i<n; i++)
    {
      getline(cin, line);
      ssin.clear();
      ssin << line;
      while(ssin >> label)
	{
	  Patg.labelVertex(i, label);
	}
    }
  
  cin >> n;
  for(i=0; i<n; i++)
    {
      cin >> s >> t >> bd;
      Patg.addEdge(s, t, bd);
    }

  cin >> n;
  for(i=0; i<Patg.vertexNum(); i++)
    {
      lmatch[i]=new int[n];
    }

  getchar();
  Datg.initGraph(n);
  for(i=0; i<n; i++)
    {
      getline(cin, line);
      ssin.clear();
      ssin << line;
      while(ssin >> label)
	{
	  Datg.labelVertex(i, label);
	}
    }

  cin >> n;
  for(i=0; i<n; i++)
    {
      cin >> s >> t >> bd;
      Datg.addEdge(s, t, bd);
    }
  
  for(i=0; i<Patg.vertexNum(); i++)
    {
      outdeg=0;
      for(k=0; k<Patg.vertexNum(); k++)
	{
	  if(Patg.adjmatrix[i][k] != -2)
	    {
	      outdeg =1;
	      break;
	    }
	}
      for(j=0; j<Datg.vertexNum(); j++)
	{
	  if(outdeg != 0 && Datg.adjlist[j].size() == 0)
	    lmatch[i][j]=0;
	  else 
	    {
	      lmatch[i][j] = labelMatch(Patg.vlabel[i], Datg.vlabel[j]);
	    }
	}
    }
  
}

int initAnc()
{
  int i, j, h, k;
  node nod;
  for(i=0; i<Patg.vertexNum(); i++)
    {
      for(j=0; j<Patg.vertexNum(); j++)
	{
	  if(i == j)
	    continue;
	  for(h=0; h<Datg.vertexNum(); h++)
	    {
	      if(!lmatch[i][h])
		continue;
	      mat[i].insert(h);

	      nod.ps=i;
	      nod.pt=j;
	      for(k=0; k<Datg.vertexNum(); k++)
		{
		  if(h == k)
		    continue;
		  nod.gn=h;
		  if(!lmatch[nod.pt][k])
		    continue;
		  mat[nod.pt].insert(k);
		  if(Datg.distance[h][k] <= Patg.adjmatrix[i][j] || Patg.adjmatrix[i][j] == -1)
		    {
		      anc[nod].insert(k);
		      nod.gn=k;
		      desc[nod].insert(h);
		    }
		}
	    }
	}
    }
  for(i=0; i<Patg.vertexNum(); i++)
    {
      if(mat[i].size() == 0)
	return 0;
    }
  return 1;
}

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

int getintersect(set<int>&s1, set<int>&s2, int sr, set<node>&pmv)
{
  node nod;
  nod.ps=sr;
  nod.gn=0;
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
	{
	  nod.pt=*ite1;
	  pmv.insert(nod);
	  ite1++;
	  ite2++;
	}
    }
  return 1;
}

int patternMatchv0()
{
  int i, j, nochg;
  node nod;
  set<int>::iterator matite;
  set<int>::iterator delite;

  while(1)
    {
      nochg=1;
      for(i=0; i<Patg.vertexNum(); i++)
	{
	  for(matite=mat[i].begin(); matite != mat[i].end(); matite++)
	    {
	      for(j=0; j<Patg.vertexNum(); j++)
		{
		  if(i == j || Patg.adjmatrix[i][j] == -2)
		    continue;
		  nod.ps=i;
		  nod.pt=j;
		  nod.gn=*matite;
		  if(!anc.count(nod) || !intersect(anc[nod], mat[j]))
		    {
		      matdel[i].insert(*matite);
		      nochg=0;
		    }
		}
	    }
	}
      if(nochg)
	break;
      for(i=0; i<Patg.vertexNum(); i++)
	{
	  for(delite=matdel[i].begin(); delite!=matdel[i].end(); delite++)
	    {
	      mat[i].erase(*delite);
	    }
	  if(mat[i].size() == 0)
	    {
	      return 0;
	    }
	}
    }
  for(i=0; i<Patg.vertexNum(); i++)
    {
      for(matite=mat[i].begin(); matite != mat[i].end(); matite++)
	{
	  cout << i << " " << *matite << endl;
	}
    }

}

int patternMatchv1()
{
  int i, j, nochg;
  node nod;
  set<int>::iterator matite;
  set<int>::iterator delite;
  set<node>::iterator pmvite;
  for(i=0; i<Patg.vertexNum(); i++)
    {
      for(matite=mat[i].begin(); matite != mat[i].end(); matite++)
	{
	  for(j=0; j<Patg.vertexNum(); j++)
	    {
	      if(i == j || Patg.adjmatrix[i][j] == -2)
		continue;
	      nod.ps=i;
	      nod.pt=j;
	      nod.gn=*matite;
	      if(!anc.count(nod) || !intersect(anc[nod], mat[j]))
		{
		  matdel[i].insert(*matite);
		}
	    }
	}
      
    }

  while(1)
    {
      for(i=0; i<Patg.vertexNum(); i++)
	{
	  for(delite=matdel[i].begin(); delite!=matdel[i].end(); delite++)
	    {
	      mat[i].erase(*delite);
	    }
	  if(mat[i].size() == 0)
	    {
	      return 0;
	    }
	}
      for(i=0; i<Patg.vertexNum(); i++)
	{
	  for(delite=matdel[i].begin(); delite!=matdel[i].end(); delite++)
	    {
	      for(j=0; j<Patg.vertexNum(); j++)
		{
		  if(i == j || Patg.adjmatrix[j][i] == -2)
		    continue;
		  nod.ps=j;
		  nod.pt=i;
		  nod.gn=*delite;
		  if(!desc.count(nod))
		    continue;
		  else 
		    {
		      getintersect(desc[nod], mat[j], j, premv);
		    }
		}
	    }
	  matdel[i].clear();
	}
      nochg=1;
      for(pmvite=premv.begin(); pmvite!=premv.end(); pmvite++)
	{
	  for(i=0; i<Patg.vertexNum(); i++)
	    {
	      if(i == pmvite->ps || Patg.adjmatrix[pmvite->ps][i] == -2)
		continue;
	      nod.ps=pmvite->ps;
	      nod.pt=i;
	      nod.gn=pmvite->pt;
	      if(!anc.count(nod) || !intersect(anc[nod], mat[nod.pt]))
		{
		  matdel[pmvite->ps].insert(nod.gn);
		  nochg=0;
		}
	    }
	}
      if(nochg)
	break;
    }

  for(i=0; i<Patg.vertexNum(); i++)
    {
      for(matite=mat[i].begin(); matite != mat[i].end(); matite++)
	{
	  cout << i << " " << *matite << endl;
	}
    }

  return 0;
}

int main()
{
  
  hbasetransport->open();

  initGraph();
  Datg.allDistance();
  if(!initAnc())
    {
      return 0;
    }
  patternMatchv0();
  
  hbasetransport->close();
  /*
    map<node, set<int> >::iterator ite;
    set<int>::iterator ite2;
    set<int>s2;
    
    for(ite=anc.begin(); ite!= anc.end(); ite++)
    {
    s2=ite->second;
    
    if(s2.size() == 0)
    continue;
    cout << ite->first.ps << " " << ite->first.pt << " " << ite->first.gn << endl;
    for(ite2=s2.begin();ite2 != s2.end(); ite2++)
    {
    cout << *ite2 << " ";
    }
    cout << endl;
    }
  */

  /*
    set<int>::iterator ite3;
    for(int i=0; i<Patg.vertexNum(); i++)
    {
    cout << i << " ";
    for(ite3 = mat[i].begin(); ite3 != mat[i].end(); ite3++)
    {
    cout << *ite3 << " ";
    }
    cout << endl;
    }
  */
  
}

