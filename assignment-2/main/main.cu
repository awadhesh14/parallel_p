#include <bits/stdc++.h>
#define THREADS_PER_BLOCK 512
#include "read_matrix2.h"
#include "printtofile.h"
#include "topology_driven.h"
#include "data_driven.h"
#include "sequential.h"
using namespace std;


int main(void){

  // ofstream op;
  // string op_file = "intermediate";
  // op.open(op_file.c_str());
  // op<<"hello"<<endl;
  // op.close();
  int M,nz; // M is no of vertices and nz is no of edges
  int i,j; //for iterator

  struct Edge *edges; //to hold the information of all the edges
  edges = read_edges("../input/test.input",&M,&nz);
   // edges = read_edges("../input/USA-road-d.CAL.gr",&M,&nz); //reads the input file in gz format into edges
                                                          //also reads the values of M and nz from input graph
  vector<vector<pair<int,int> > > adj; //adjency list represenattion of graph
  adj.resize(M); // resize the main holder array to no of vertices

  for (i=0;i<nz;i++){ //populating the adjency list
    adj[edges[i].src].push_back(make_pair(edges[i].sink,edges[i].wght));
  }

  int *sources = new int[M+1]; //this is like CSR ind_ptr edges[i]=sources[i+1]-sources[i]
                              //to find neighbors of vertex i, do neigh=sources[i];neigh<sources[i+1]
  int *sinks = new int[nz]; // sinks[sources[i]] to sinks[sources[i+1]-1] belongs to vertex i
  float *wghts = new float[nz];// same as above has weights for them sinks

  sources[0] = 0; // initialize to zero, same as ind_ptr in CSR
  int it,jt; //iterators
  for(it=0,i=1,j=0 ; it<adj.size() ; it++,i++){ //populating the above 3 from the adjency list
    sources[i] = adj[it].size() + sources[i-1];
    for(jt=0 ; jt<adj[it].size() ; jt++){
        sinks[j] = (adj[it].at(jt)).first;
        wghts[j++] = (adj[it].at(jt)).second;
    }
  }
  // for(i=0;i<M+1;i++)
  //   cout<<sources[i]<<" ";
  // cout<<endl;
  // for(i=0;i<nz;i++)
  //   cout<<sinks[i]<<" "<<wghts[i]<<endl;

  //float *dist;// = new float[M]; //distances of all vertices from source, vertex is represented by index

  cout<<"takatak"<<endl;
  sequential (M,nz,edges);
  topology_driven(M,nz,sources,sinks,wghts);
  data_driven(M,nz,sources,sinks,wghts);


  printf("everything is said and done\n");



  return 0;
}
