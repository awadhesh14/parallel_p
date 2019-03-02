#include <bits/stdc++.h>
#include "read_matrix2.h"
using namespace std;


int main(void){

  int M,nz; // M is no of vertices and nz is no of edges
  int i,j; //for iterator

  struct Edge *edges; //to hold the information of all the edges
  edges = read_edges("../input/USA-road-d.CAL.gr",&M,&nz); //reads the input file in gz format into edges
                                                          //also reads the values of M and nz from input graph
  vector<vector<pair<int,int> > > adj; //adjency list represenattion of graph
  adj.resize(M); // resize the main holder array to no of vertices

  for (i=0;i<nz;i++){ //populating the adjency list
    adj[edges[i].src].push_back(make_pair(edges[i].sink,edges[i].wght));
  }

  int *sources = new int[M+1];
  int *sinks = new int[nz];
  double *wghts = new double[nz];

  sources[0] = 0;
  for(vector<vector<pair<int,int> > >::iterator it=adj.begin(),i=1,j=0;it!=adj.end();it++,i++){
    sources[i] = it.size() + sources[i-1];
    for(vector<pair<int,int> > ::iterator jt=it.begin();jt!=it.end();jt++){
        sinks[j] = (*jt).first;
        wghts[j++] = (*jt).second;
    }
  }

  printf("everything is said and done\n");

  return 0;
}
