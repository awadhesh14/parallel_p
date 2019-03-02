#include <bits/stdc++.h>
#include "read_matrix2.h"

int main(void){
  int M,nz;
  int i;

  struct Edge *edges;// = new Edge[nz+1];
  edges = read_edges("../input/USA-road-d.CAL.gr",&M,&nz);
  // edges = read_edges("../input/test.input",&M,&nz);
  double *dist = new double[M];

  for(i=0;i<M;i++){
    dist[i]=DBL_MAX;
  }
  dist[50000] = 0;

  int change;
  for(i=0;i<M;i++){
    change = 0;
    for(int j=0;j<nz;j++){
      if(dist[edges[j].src] + edges[j].wght < dist[edges[j].sink] ){
        change = 1;
        dist[edges[j].sink] = dist[edges[j].src] + edges[j].wght;
      }
    }
    if(change==0)
      break;

  }
  for(i=0;i<M;i++)
    fprintf(stdout, "%lg\n", dist[i]);
	return 0;
}
