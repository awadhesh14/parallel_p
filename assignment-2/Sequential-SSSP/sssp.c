#include <bits/stdc++.h>
#include "read_matrix.h"

int main(void){
  int M, N, nz;
  int i, *I, *J;
  double *val;


  /*this function is from sequencial-SSSP/read_matrix.h
  * It reads values of M,N,nz
  * And it will read I,J,val with zero indexing(not 1-indexing as in MM)
  */
  //read_matrix("../input/webbase-1M/webbase-1M.mtx",&M,&N,&nz,I,J,val);

  read_M_N_nz("../input/webbase-1M/webbase-1M.mtx",&M,&N,&nz);
  printf("%d %d %d\n  ",M,N,nz);
  struct Edge *edges = new Edge[nz+1];
  read_Mat("../input/webbase-1M/webbase-1M.mtx",M,N,nz,edges);
  // for(i=0 ; i<nz ; i++)
  //   printf("%d %d %d %lg\n",i,edges[i].src,edges[i].sink,edges[i].wght);
  double *dist = new double[M];
  std::memset (dist,std::numeric_limits<double>::infinity(),sizeof dist);
  dist[0] = 0;
  // std::priority_queue<double, std::vector<double>, std::greater<double> >q (dist,dist+nz);

  for(i=0;i<M;i++)
    for(int j=0;j<nz;j++)
      if(dist[edges[j].src] + edges[j].wght < dist[edges[j].sink] )
        dist[edges[j].sink] = dist[edges[j].src] + edges[j].wght;

	return 0;
}
