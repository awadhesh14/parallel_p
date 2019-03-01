#include <bits/stdc++.h>
#include "read_matrix2.h"

int main(void){
  int M, N, nz;
  int i, *I, *J;
  double *val;


  // read_V_E("../input/USA-road-d.CAL.gr",&M,&nz);
  // printf("%d %d \n",M,nz);

  struct Edge *edges;// = new Edge[nz+1];
  edges = read_edges("../input/USA-road-d.CAL.gr",&M,&nz);
  // edges = read_edges("../input/test.input",&M,&nz);
  double *dist = new double[M];
  // std::memset (dist,std::numeric_limits<double>::infinity(),sizeof dist);
  for(i=0;i<M;i++){
    dist[i]=DBL_MAX;
    // fprintf(stdout, "%lg\n", dist[i]);
  }
  dist[0] = 0;
  // std::priority_queue<double, std::vector<double>, std::greater<double> >q (dist,dist+nz);
  int change;
  // for(i=0;i<nz;i++){
  //   printf("%d %d %lg\n",edges[i].src,edges[i].sink,edges[i].wght);
  // }
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
